#include "WebServ.hpp"

//TODO cleanup some of the functions (maybe add some smaller functions)
//TODO throw for somethings and make a error handler for those

// Constructors
WebServ::WebServ(Config *config) : _config(config)
{
	t_servmap	server_map;

	// Starting the kqueue
	if ((_kqueue = kqueue()) == -1)
		std::cout << "Error: kqueue failed" << std::endl;
	server_map = _config->getServerMap();
	_n_servers = 0;
	// Going through the config and making a socket and event for all servers in it.
	for (auto it = server_map.begin(); it != server_map.end(); it++)
	{
		std::cout << "setting socket for: " << it->first << std::endl;
		setNewServerSocket(it->second);
		_n_servers++;
	}
	//This will make it so that kqueue will look for changes to the server events
	kevent(_kqueue, &_change_ev[0], _change_ev.size(), NULL, 0, NULL);
}

WebServ::WebServ(const WebServ &copy)
{
	(void) copy;
}


// Destructor
WebServ::~WebServ()
{
}


// Operators
WebServ & WebServ::operator=(const WebServ &assign)
{
	(void) assign;
	return *this;
}

// Getters
Server *WebServ::getServer(std::string key) const
{
	return _config->getServerMap().find(key)->second;
}

// Member Functions
void	WebServ::setNewServerSocket(Server *server)
{
	int			srvr_sckt;
	int			option = 1;
	t_event		event;
	t_evudat	*ev_udat = new t_evudat;
	t_addr_in	srvr_addr;
	
	// Creating the socket
	srvr_sckt = socket(AF_INET, SOCK_STREAM, 0);
	if (srvr_sckt == -1)
	{
		std::cout << "Error: socket failed" << std::endl;
		perror("socket");
	}
	server->setServerSocket(srvr_sckt);
	setsockopt(srvr_sckt, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	// Setting the address struct
	memset((char *)&srvr_addr, 0, sizeof(srvr_addr));
	srvr_addr.sin_family = AF_INET;
	srvr_addr.sin_addr.s_addr = inet_addr(server->getServerIp().c_str());
	srvr_addr.sin_port = htons(server->getServerPort());

	// Binding and listening to the new socket using the address struct data
	if (bind(srvr_sckt, (t_sckadr *)&srvr_addr, sizeof(srvr_addr)) == -1)
	{
		std::cout << "Error: bind failed" << std::endl;
		perror("bind");
	}
	if (listen(srvr_sckt, BACKLOG) == -1)
	{
		std::cout << "Error: listen() failed" << std::endl;
		perror("listen");
	}
	
	// Setting the server socket as nonblocking
	fcntl(srvr_sckt, F_SETFL, O_NONBLOCK);

	//Setting the udata for the event and adding it to the changelst.
	ev_udat->ip = server->getServerIp();
	ev_udat->port = server->getServerPort();
	ev_udat->key = server->getServerIp() + ":" + std::to_string(server->getServerPort());
	EV_SET(&event, srvr_sckt, EVFILT_READ, EV_ADD, 0, 0, ev_udat);
	_change_ev.push_back(event);
}

void	WebServ::deleteConnection(t_event event, int16_t	filter)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	std::cout << "going to delete a connection/event" << std::endl;
	EV_SET(&event, event.ident, filter, EV_DELETE, 0, 0, evudat);
	kevent(_kqueue, &event, 1, NULL, 0, NULL);
	if (evudat->flag)
		close(event.ident);
	else
		evudat->flag = true;
	std::cout << "deleted a connection/event" << std::endl;
}

void	WebServ::addConnection(t_event event, t_evudat *old_udat)
{
	int			clnt_sckt;
	int			opt_value = 1;
	t_addr_in	newaddr;
	socklen_t	socklen = sizeof(newaddr);
	char		host[1024];
	char		service[20];

	clnt_sckt = accept(event.ident, (t_sckadr *)(&newaddr), &socklen);
	if (clnt_sckt == -1)
	{
		std::cout << "accept error" << std::endl;
		perror("accept");
	}
	setsockopt(clnt_sckt, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));

	//setting initial values for the new_udat
	t_evudat	*new_udat = new t_evudat;
	new_udat->flag = false;
	new_udat->addr = newaddr;
	new_udat->ip = old_udat->ip;
	new_udat->port = old_udat->port;
	new_udat->key = old_udat->key;
	new_udat->req = new RequestHandler(getServer(new_udat->key));
	new_udat->req->setSocket(clnt_sckt);
	getnameinfo((const t_sckadr *)&newaddr, socklen, host, sizeof host, service, sizeof service, 0);

	//putting the read and write event for the new client in the kqueue
	t_event		new_event[2];
	EV_SET(&new_event[0], clnt_sckt, EVFILT_READ, EV_ADD, 0, 0, new_udat);
	EV_SET(&new_event[1], clnt_sckt, EVFILT_WRITE, EV_ADD, 0, 0, new_udat);
	kevent(_kqueue, new_event, 2, NULL, 0, NULL);

	//Debug messages
	// std::cout << "   host: " << host << std::endl;
	// std::cout << "service: " << service << std::endl;
	std::cout << "Added new client connecting from ip: " << inet_ntoa(newaddr.sin_addr);
	std::cout << " and port: " << ntohs(newaddr.sin_port) << std::endl;
	std::cout << "Client connected to server with ip: " << old_udat->ip << " and port: " << old_udat->port << std::endl;
}

//TODO requests need to be parsed and handled still
void	WebServ::receiveRequest(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;
	char		buf[MAX_MSG_SIZE];
	int			bytes_read;

	bytes_read = recv(event.ident, buf, sizeof(buf) - 1, 0);
	if (bytes_read < 0)
	{
		std::cout << "receive error" << std::endl;
	}
	else if (bytes_read == 0)
	{
		evudat->flag = true;
		// std::cout << "bytes_read is 0" << std::endl;
	}
	else
	{
		buf[bytes_read] = 0;
		evudat->req->addToRequestMsg(buf);
	}
	// fflush(stdout);
	// Receive message might also contain part of the next packet
	// TODO make sure this is handled in the requesthandler!.
}

void	WebServ::sendResponse(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;
	std::string	response;
	std::string	remaining_request;

	// get response
	response = evudat->req->getResponse();

	// In case receive receives more than one packet
	if (evudat->req->hasRemainingRequestMsg())
	{
		// std::cout << "should have remaining request here" << std::endl;
		remaining_request = evudat->req->getRemainingRequestMsg();
	}
	// Send respons
	if (send(event.ident, response.c_str(), response.length(), 0) == -1)
	{
		std::cout << "Error sending response" << std::endl;
		return ;
	}
	//TODO have a sendall function (so you know that the whole message has been sent)
	//Make a new RequestHandler
	delete evudat->req;
	evudat->req = new RequestHandler(getServer(evudat->key));
	if (evudat->req->hasRemainingRequestMsg())
	{
		evudat->req->addToRequestMsg(remaining_request);
		std::cout << "\n------Adding remaining request to new RequestHandler--------" << std::endl;
	}


	// send(client_socket, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 100\n\n", 62, 0);
	// send(client_socket, "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>My First Heading</h1>\n<p>My first paragraph.</p>\n\n</body>\n</html>", 100, 0);
}

bool	WebServ::isListenSocket(int fd)
{
	for (int i = 0; i < _n_servers; i++)
		if (_change_ev[i].ident == (uintptr_t)fd)
			return 1;
	return 0;
}

void	WebServ::readFromSocket(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	if (event.flags & EV_EOF || evudat->flag)
		deleteConnection(event, EVFILT_READ);
	else
		receiveRequest(event);
}

void	WebServ::writeToSocket(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	if (event.flags & EV_EOF || evudat->flag)
		deleteConnection(event, EVFILT_WRITE);
	else if (evudat->req->isRequestComplete())
		sendResponse(event);
}

void	WebServ::runServer()
{
	int		new_evnt;
	t_event	events[MAX_EVENTS];

	while (true)
	{
		new_evnt = kevent(_kqueue, NULL, 0, events, _n_servers, NULL);
		if (new_evnt == -1)
			std::cout << "Error: kevent failure" << std::endl;
		for (int i = 0; i < new_evnt; i++)
		{
			if (events[i].flags & EV_ERROR)
				std::cout << "Error: Socket got deleted" << std::endl;
			if (isListenSocket(events[i].ident))
				addConnection(events[i], (t_evudat *)(events[i].udata));
			else if (events[i].filter == EVFILT_READ)
				readFromSocket(events[i]);
			else if (events[i].filter == EVFILT_WRITE)
				writeToSocket(events[i]);
		}
	}
}
