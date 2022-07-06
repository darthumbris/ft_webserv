#include "WebServ.hpp"

// Constructors

//TODO make this properly work with the config class
WebServ::WebServ(Config *config)
{
	std::map<std::string, Server*>	server;

	// _clients.resize(MAX_FD);
	// _server_fd.resize(MAX_FD);
	if ((_kqueue = kqueue()) == -1)
		std::cout << "Error: kqueue failed" << std::endl;
	server = config->getServerMap();
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
	EV_SET(&event, srvr_sckt, EVFILT_READ, EV_ADD, 0, 0, ev_udat);
	std::cout << "server socket: " << srvr_sckt << std::endl;
	_change_ev.push_back(event);
}

// Member Functions
void	WebServ::deleteConnection(struct kevent event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	EV_SET(&event, event.ident, EVFILT_READ, EV_DELETE, 0, 0, evudat);
	kevent(_kqueue, &event, 1, NULL, 0, NULL);
	std::cout << "deleting connection" << std::endl;
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
	EV_SET(&new_event[0], client_socket, EVFILT_READ, EV_ADD, 0, 0, new_data);
	EV_SET(&new_event[1], client_socket, EVFILT_WRITE, EV_ADD, 0, 0, new_data);
	kevent(_kqueue, new_event, 2, NULL, 0, NULL);
	std::cout << "Added new client connecting from ip: " << inet_ntoa(newaddr.sin_addr) << std::endl;
	std::cout << "Client connected to server with ip: " << old_data->ip << " and port: " << old_data->port << std::endl;
}

void	WebServ::receiveRequest(int client_socket)
{
	char	buf[MAX_MSG_SIZE];
	int		bytes_read;

	bytes_read = recv(client_socket, buf, sizeof(buf) - 1, 0);
	if (bytes_read < 0)
		return ;
	buf[bytes_read] = 0;
	if (strnstr(buf, "GET / HTTP/1.1", strlen(buf)))
		send(client_socket, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!", 74, 0);
	else
		std::cout << buf;
	fflush(stdout);
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

void	WebServ::readSocket(struct kevent &event)
{
	t_evudat	*evudat = (t_evudat *)event.udata;

	if (event.flags & EV_EOF || evudat->flag)
		deleteConnection(event);
	else
		receiveRequest(event.ident);
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
				readSocket(events[i]);
			else if (events[i].filter == EVFILT_WRITE)
			{
				// writeSocket();
			}			
		}
	}
}
