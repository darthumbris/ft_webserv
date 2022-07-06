#include "WebServ.hpp"

// Constructors

//TODO make this properly work with the config class
WebServ::WebServ(Config *config) : _config(config)
{
	std::map<std::string, Server*>	server;

	// _clients.resize(MAX_FD);
	// _server_fd.resize(MAX_FD);
	if ((_kqueue = kqueue()) == -1)
		std::cout << "Error: kqueue failed" << std::endl;
	server = _config->getServerMap();
	_n_servers = 0;
	for (std::map<std::string, Server*>::iterator it = server.begin(); it != server.end(); it++)
	{
		std::cout << "setting socket for: " << it->first << std::endl;
		setNewServerSocket(it->second);
		_n_servers++;
	}
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

// Setters

//TODO doesn't properly set the srvr_fds for now
void	WebServ::setNewServerSocket(Server *server)
{
	int					srvr_sckt;
	struct sockaddr_in	srvr_addr;
	struct kevent		event;
	int					option;
	t_evudat			*ev_udat = new t_evudat;
	
	memset((char *)&srvr_addr, 0, sizeof(srvr_addr));
	srvr_sckt = socket(AF_INET, SOCK_STREAM, 0);
	if (srvr_sckt == -1)
		std::cout << "Error: socket failed" << std::endl;
	server->setServerSocket(srvr_sckt);
	option = 1;
	setsockopt(srvr_sckt, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	srvr_addr.sin_family = AF_INET;
	srvr_addr.sin_addr.s_addr = inet_addr(server->getServerIp().c_str());
	srvr_addr.sin_port = htons(server->getServerPort());
	if (bind(srvr_sckt, (sckadr)&srvr_addr, sizeof(srvr_addr)) == -1)
		std::cout << "Error: bind failed" << std::endl;
	if (listen(srvr_sckt, BACKLOG) == -1)
		std::cout << "Error: listen() failed" << std::endl;
	fcntl(srvr_sckt, F_SETFL, O_NONBLOCK);
	ev_udat->ip = server->getServerIp();
	ev_udat->port = server->getServerPort();
	ev_udat->srvr_key = server->getServerIp() + ":" + std::to_string(server->getServerPort());
	EV_SET(&event, srvr_sckt, EVFILT_READ, EV_ADD, 0, 0, ev_udat);
	_change_ev.push_back(event);
}

// Member Functions
void	WebServ::deleteConnection(struct kevent event, int16_t	filter)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	EV_SET(&event, event.ident, filter, EV_DELETE, 0, 0, evudat);
	kevent(_kqueue, &event, 1, NULL, 0, NULL);
	// std::cout << "deleting connection" << std::endl;
	if (evudat->flag)
	{
		shutdown(event.ident, 0);
		close(event.ident);
	}
	else
		evudat->flag = 1;
}

void	WebServ::addConnection(struct kevent event)
{
	struct sockaddr_in	newaddr;
	socklen_t			socklen = sizeof(newaddr);
	int					client_socket;
	int					opt;
	struct kevent		new_event[2];
	t_evudat			*old_data = (t_evudat *)(event.udata);
	t_evudat			*new_data = new t_evudat;

	client_socket = accept(event.ident, (sckadr)(&newaddr), &socklen);
	opt = 1;
	setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	new_data->flag = 0;
	new_data->addr = newaddr;
	new_data->ip = old_data->ip;
	new_data->port = old_data->port;
	new_data->srvr_key = old_data->srvr_key;
	new_data->req = new RequestHandler(_config->getServerMap().find(new_data->srvr_key)->second); //TODO make sure this works
	EV_SET(&new_event[0], client_socket, EVFILT_READ, EV_ADD, 0, 0, new_data);
	EV_SET(&new_event[1], client_socket, EVFILT_WRITE, EV_ADD, 0, 0, new_data);
	kevent(_kqueue, new_event, 2, NULL, 0, NULL);
	// std::cout << "Added new client connecting from ip: " << inet_ntoa(newaddr.sin_addr) << std::endl;
	// std::cout << "Client connected to server with ip: " << old_data->ip << " and port: " << old_data->port << std::endl;
}

//TODO requests need to be parsed and handled still
void	WebServ::receiveRequest(struct kevent &event)
{
	char	buf[MAX_MSG_SIZE];
	int		bytes_read;

	bytes_read = recv(event.ident, buf, sizeof(buf) - 1, 0);
	if (bytes_read < 0)
		return ;
	buf[bytes_read] = 0;
	if (strnstr(buf, "GET / HTTP/1.1", strlen(buf))) //just for testing stuff
	{
		send(event.ident, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 100\n\n", 62, 0);
		send(event.ident, "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>My First Heading</h1>\n<p>My first paragraph.</p>\n\n</body>\n</html>", 100, 0);
	}
	std::cout << buf; //for debug and understanding http headers
	fflush(stdout);
}

void	WebServ::sendResponse(struct kevent &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	delete evudat->req;
	evudat->req = new RequestHandler(_config->getServerMap().find(evudat->srvr_key)->second);
	//TODO probably update the request in the udata of the event
	// send(client_socket, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 100\n\n", 62, 0);
	// send(client_socket, "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>My First Heading</h1>\n<p>My first paragraph.</p>\n\n</body>\n</html>", 100, 0);
}

bool	WebServ::isListenSocket(int fd)
{
	for (int i = 0; i < _n_servers; i++)
	{
		if (_change_ev[i].ident == (uintptr_t)fd)
			return 1;
	}
	return 0;
}

void	WebServ::readFromSocket(struct kevent &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	if (event.flags & EV_EOF || evudat->flag)
		deleteConnection(event, EVFILT_READ);
	else
		receiveRequest(event);
}

void	WebServ::writeToSocket(struct kevent &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	if (event.flags & EV_EOF || evudat->flag)
		deleteConnection(event, EVFILT_WRITE);
	else
	{
		sendResponse(event);
	}
}

//TODO make it work with the current setup
void	WebServ::runServer()
{
	int				new_evnt;
	struct kevent	events[MAX_EVENTS];

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
				addConnection(events[i]);
			else if (events[i].filter == EVFILT_READ)
				readFromSocket(events[i]);
			else if (events[i].filter == EVFILT_WRITE)
				writeToSocket(events[i]);	
		}
	}
}
