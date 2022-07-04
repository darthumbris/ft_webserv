#include "WebServ.hpp"

// Constructors
WebServ::WebServ(int port, char *address)
{
	memset((char *)&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(address);
	_address.sin_port = htons(port);
	_srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(_srv_fd, (struct sockaddr *)&_address, sizeof(_address));
	listen(_srv_fd, BACKLOG);
	fcntl(_srv_fd, F_SETFL, O_NONBLOCK);
	_kqueue = kqueue();
	EV_SET(&_ev_set, _srv_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(_kqueue, &_ev_set, 1, NULL, 0, NULL);
	_socklen = sizeof(_addr);
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
	int	i;

	i = -1;
	while (++i < NUM_CLIENTS)
		if (clients[i].fd == client_socket)
			return (i);
	return (-1);
}

// Member Functions
int	WebServ::deleteConnection(int client_socket)
{
	int	i;

	eventHandler(1);
	if (client_socket < 1)
		return (-1);
	i = this->getConnectionId(client_socket);
	if (i == -1)
		return (-1);
	clients[i].fd = 0;
	shutdown(clients[i].fd, 0);
	std::cout << "Client #"<< i << "disconnected." << std::endl;
	
	return (close(client_socket));
}

int	WebServ::addConnection(int client_socket)
{
	int	i;

	if (client_socket < 1)
		return (-1);
	i = this->getConnectionId(0);
	if (i == -1)
		return (-1);
	clients[i].fd = client_socket;
	return (0);
}

void	WebServ::receiveMsg(int client_socket)
{
	char	buf[MAX_MSG_SIZE];
	int		bytes_read;

	bytes_read = recv(client_socket, buf, sizeof(buf) - 1, 0);
	buf[bytes_read] = 0;
	if (strnstr(buf, "GET / HTTP/1.1", MAX_MSG_SIZE))
		send(client_socket, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!", 74, 0);
	std::cout << buf;
	fflush(stdout);
}

void	WebServ::refuseConnection(int client_socket)
{
	std::cout << "connection refused." << std::endl;
	close(client_socket);
}

void	WebServ::eventHandler(int event_type)
{
	switch (event_type)
	{
	case 0:
		EV_SET(&_ev_set_run, _clnt_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		break;
	case 1:
		EV_SET(&_ev_set_run, _clnt_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		break;
	default:
		break;
	}
	kevent(_kqueue, &_ev_set_run, 1, NULL, 0, NULL);
}

void	WebServ::runServer()
{
	_n_events = kevent(_kqueue, NULL, 0, _ev_lst, MAX_EVENTS, NULL);
	for (int i = 0; i < _n_events; i++)
	{
		if (_ev_lst[i].ident == (uintptr_t)_srv_fd)
		{
			_clnt_fd = accept(_ev_lst[i].ident, (struct sockaddr *)&_addr, &_socklen);
			if (addConnection(_clnt_fd) == 0)
				eventHandler(0);
			else
				refuseConnection(_clnt_fd);
		}
		else if (_ev_lst[i].flags & EV_EOF)
			deleteConnection(_ev_lst[i].ident);
		else if (_ev_lst[i].filter == EVFILT_READ)
			receiveMsg(_ev_lst[i].ident);
	}
}
