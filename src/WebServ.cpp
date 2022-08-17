#include "WebServ.hpp"

//TODO check for memory leaks and proper delete etc usage
//TODO make a index.html that shows all stuff we need to handle (file uploading redirect etc)

// Constructors
WebServ::WebServ(Config *config) : _config(config)
{
	// Starting the kqueue
	if ((_kqueue = kqueue()) == -1)
		std::cout << "Error: kqueue failed" << std::endl;
	t_servmap server_map = _config->getServerMap();
	_n_servers = 0;

	// Going through the config and making a socket and event for all servers in it.
	std::cout << "\n\nServers loaded from config: " << server_map.size() << "\n" << std::endl;
	for (std::size_t it = 0; it < server_map.size(); it++)
	{
		std::vector<int> ports = server_map[it].getServerPort();
		for (std::size_t i = 0; i < ports.size(); i++)
		{
			if (!listeningToPort(ports[i]))
			{
				std::cout << "setting socket for port: " << ports[i] << std::endl;
				try	{setNewServerSocket(server_map[it], ports[i]);}
				catch(const std::exception& e) {std::cerr << e.what() << std::endl;}
				addPortToList(ports[i]);
				_n_servers++;
			}
		}
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
bool	WebServ::listeningToPort(int port) const
{
	for (std::size_t it = 0; it < _ports.size(); it++)
	{
		if (_ports[it] == port)
			return true;
	}
	return false;
}

//Setters
void	WebServ::addPortToList(int port)
{
	this->_ports.push_back(port);
}

// Member Functions
void	WebServ::setNewServerSocket(Server server, int port)
{
	int			srvr_sckt;
	int			option = 1;
	t_event		event;
	t_evudat	*ev_udat = new t_evudat;
	t_addr_in	srvr_addr;
	
	// Creating the socket
	srvr_sckt = socket(AF_INET, SOCK_STREAM, 0);
	if (srvr_sckt == -1)
		throw WebServerExcpetion{"Error: creating socket for server failed"};
	server.setServerSocket(srvr_sckt);
	setsockopt(srvr_sckt, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	// Setting the address struct
	memset((char *)&srvr_addr, 0, sizeof(srvr_addr));
	srvr_addr.sin_family = AF_INET;
	srvr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srvr_addr.sin_port = htons(port);

	// Binding and listening to the new socket using the address struct data
	if (bind(srvr_sckt, (t_sckadr *)&srvr_addr, sizeof(srvr_addr)) == -1)
		throw WebServerExcpetion{"Error: bind() failed"};
	if (listen(srvr_sckt, BACKLOG) == -1)
		throw WebServerExcpetion{"Error: listen() failed"};
	
	// Setting the server socket as nonblocking
	fcntl(srvr_sckt, F_SETFL, O_NONBLOCK);

	//Setting the udata for the event and adding it to the changelst.
	ev_udat->ip = server.getServerIp();
	ev_udat->port = port;
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
		evudat->flag = 1;
}

//TODO maybe needs a check for the udat if it needs to be deleted?
void	WebServ::addConnection(t_event event, t_evudat *old_udat)
{
	int			opt_value = 1;
	t_addr_in	newaddr;
	socklen_t	socklen = sizeof(newaddr);

	int clnt_sckt = accept(event.ident, (t_sckadr *)(&newaddr), &socklen);
	if (clnt_sckt == -1)
		throw WebServerExcpetion{"Error: accept() failed"};
	setsockopt(clnt_sckt, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));

	//setting initial values for the new_udat
	t_evudat	*new_udat = new t_evudat;
	memset(new_udat, 0, sizeof(t_evudat));
	new_udat->addr = newaddr;
	new_udat->ip = old_udat->ip;
	new_udat->port = old_udat->port;
	new_udat->req = new RequestHandler(_config->getServerMap());
	new_udat->req->setSocket(clnt_sckt);
	new_udat->req->setPort(old_udat->port);
	new_udat->req->setClientIp(inet_ntoa(newaddr.sin_addr));

	//putting the read and write event for the new client in the kqueue
	t_event		new_event[2];
	EV_SET(&new_event[0], clnt_sckt, EVFILT_READ, EV_ADD, 0, 0, new_udat);
	EV_SET(&new_event[1], clnt_sckt, EVFILT_WRITE, EV_ADD, 0, 0, new_udat);
	kevent(_kqueue, new_event, 2, NULL, 0, NULL);

	//Debug message
	std::cout << "New client connected to server with port: " << old_udat->port << std::endl;
}

void	WebServ::receiveRequest(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;
	char		buf[MAX_MSG_SIZE];
	int			bytes_read;
	static int	total_bytes = 0;

	bytes_read = recv(event.ident, buf, sizeof(buf) - 1, 0);
	total_bytes += bytes_read;
	if (bytes_read < 0)
		std::cout << "recv error" << std::endl;
	else if (bytes_read == 0 && evudat->flag != 2)
		evudat->flag = 1;
	else
	{
		buf[bytes_read] = 0;
		evudat->req->addToRequestMsg(buf, bytes_read);
	}
}

void	WebServ::sendResponse(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;
	std::string	response;
	int			fd;

	fd = evudat->req->getFileDescriptor();
	if (evudat->flag != 2)
	{		
		response = evudat->req->getResponse();
		send(event.ident, response.c_str(), response.size(), 0);
	}
	if (fd > 0)
	{
		std::cout << "file descriptor open" << std::endl;
		int bytes = sendfile(fd, event.ident, evudat->total_size, &evudat->datalen, NULL, 0);
		evudat->total_size += evudat->datalen;
		if (bytes == -1 || evudat->total_size < evudat->req->getFileSize())
		{
			evudat->flag = 2;
			return ;
		}
	}
	if (evudat->total_size >= evudat->req->getFileSize() || fd <= 0)
	{
		evudat->flag = 0;
		if (fd > 0)
			close(fd);
		evudat->datalen = 0;
		evudat->total_size = 0;
		delete evudat->req;
		evudat->req = new RequestHandler(_config->getServerMap());
		evudat->req->setPort(evudat->port);
	}
}

bool	WebServ::isListenSocket(int fd)
{
	for (std::size_t i = 0; i < _change_ev.size(); i++)
	{
		if (_change_ev[i].ident == (uintptr_t)fd)
			return true;
	}
	return false;
}

void	WebServ::readFromSocket(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	if (event.flags & EV_EOF || evudat->flag == 1)
		deleteConnection(event, EVFILT_READ);
	else
		receiveRequest(event);
}

void	WebServ::writeToSocket(t_event &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	if (event.flags & EV_EOF || evudat->flag == 1)
		deleteConnection(event, EVFILT_WRITE);
	else if (evudat->req->isRequestComplete())
		sendResponse(event);
}

void	WebServ::runServer()
{
	int		new_evnt;
	t_event	events[MAX_EVENTS];

	try
	{
		while (true)
		{
			new_evnt = kevent(_kqueue, NULL, 0, events, _n_servers, NULL);
			if (new_evnt == -1)
				throw WebServerExcpetion("Error: kevent failed");
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
	catch(const std::exception& e) 
	{
		std::cerr << e.what() << '\n';
	}
	exit(1);
}
