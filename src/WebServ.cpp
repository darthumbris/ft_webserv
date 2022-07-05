#include "WebServ.hpp"

// Constructors

//TODO make this properly work with the config class
WebServ::WebServ(Config *config)
{
	struct kevent	ev_set;

	memset((char *)&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	std::cout << "ip: " << config->getServerMap().at("127.0.0.1:8080")->getServerIp() << std::endl;
	_address.sin_addr.s_addr = inet_addr(config->getServerMap().at("127.0.0.1:8080")->getServerIp().c_str());
	_address.sin_port = htons(8080);
	_srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(_srv_fd, (struct sockaddr *)&_address, sizeof(_address));
	listen(_srv_fd, BACKLOG);
	fcntl(_srv_fd, F_SETFL, O_NONBLOCK);
	_kqueue = kqueue();
	EV_SET(&ev_set, _srv_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(_kqueue, &ev_set, 1, NULL, 0, NULL);
	(void)config;
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
int	WebServ::getConnectionId(int client_socket) const
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] == client_socket)
			return (i);
	}
	return (-1);
}

// Member Functions
int	WebServ::deleteConnection(int client_socket)
{
	int	i;

	eventHandler(DELETE, client_socket);
	if (client_socket < 1)
		return (-1);
	i = this->getConnectionId(client_socket);
	if (i == -1)
		return (-1);
	shutdown(_clients[i], 0);
	_clients.erase(_clients.begin() + i);
	std::cout << "Client #"<< i << " disconnected." << std::endl;
	return (close(client_socket));
}

int	WebServ::addConnection(int client_socket)
{
	if (client_socket < 1)
		return (-1);
	_clients.push_back(client_socket);
	std::cout << "Client #" << _clients.size() - 1 << " connected" << std::endl;
	return (0);
}

void	WebServ::receiveRequest(int client_socket)
{
	char	buf[MAX_MSG_SIZE];
	int		bytes_read;

	bytes_read = recv(client_socket, buf, sizeof(buf) - 1, 0);
	buf[bytes_read] = 0;
	if (strnstr(buf, "GET / HTTP/1.1", MAX_MSG_SIZE))
		send(client_socket, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!", 74, 0);
	else
		std::cout << buf;
	fflush(stdout);
}

void	WebServ::refuseConnection(int client_socket)
{
	std::cout << "connection refused." << std::endl;
	close(client_socket);
}

void	WebServ::eventHandler(int event_type, int client_socket)
{
	struct kevent	ev_set;
	switch (event_type)
	{
	case ADD:
		EV_SET(&ev_set, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
		break;
	case DELETE:
		EV_SET(&ev_set, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		break;
	default:
		break;
	}
	kevent(_kqueue, &ev_set, 1, NULL, 0, NULL);
}

//Needs some changes now it only runs one of the servers
void	WebServ::runServer()
{
	int				n_events;
	int				clnt_fd;
	t_sckadr_strg	addr_strg;
	socklen_t		socklen;
	struct timespec	timeout = {5, 0};
	

	socklen = sizeof(addr_strg);
	while (true)
	{
		n_events = kevent(_kqueue, NULL, 0, _ev_lst, MAX_EVENTS, &timeout);
		for (int i = 0; i < n_events; i++)
		{
			if (_ev_lst[i].ident == (uintptr_t)_srv_fd)
			{
				clnt_fd = accept(_ev_lst[i].ident, (sckadr)&addr_strg, &socklen);
				if (addConnection(clnt_fd) == 0)
					eventHandler(ADD, clnt_fd);
				else
					refuseConnection(clnt_fd);
			}
			else if (_ev_lst[i].flags & EV_EOF)
				deleteConnection(_ev_lst[i].ident);
			else if (_ev_lst[i].filter == EVFILT_READ)
				receiveRequest(_ev_lst[i].ident);
		}
	}
}
