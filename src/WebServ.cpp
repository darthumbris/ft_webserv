#include "WebServ.hpp"

// Constructors

//TODO make this properly work with the config class
WebServ::WebServ(Config *config)
{
	std::map<std::string, Server*>	server;

	if ((_kqueue = kqueue()) == -1)
		std::cout << "Error: kqueue failed" << std::endl;
	server = config->getServerMap();
	for (std::map<std::string, Server*>::iterator it = server.begin(); it != server.end(); it++)
	{
		std::cout << "setting socket for: " << it->first << std::endl;
		setNewServerSocket(it->second);
	}
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

// Setters

//TODO doesn't properly set the srvr_fds for now
void	WebServ::setNewServerSocket(Server *server)
{
	int					srvr_sckt;
	struct sockaddr_in	srvr_addr;
	struct kevent		event;
	
	memset((char *)&srvr_addr, 0, sizeof(srvr_addr));
	srvr_sckt = socket(AF_INET, SOCK_STREAM, 0);
	if (srvr_sckt == -1)
		std::cout << "Error: socket failed" << std::endl;
	server->setServerSocket(srvr_sckt);
	srvr_addr.sin_family = AF_INET;
	srvr_addr.sin_addr.s_addr = inet_addr(server->getServerIp().c_str());
	srvr_addr.sin_port = htons(server->getServerPort());
	if (bind(srvr_sckt, (sckadr)&srvr_addr, sizeof(srvr_addr)) == -1)
		std::cout << "Error: bind failed" << std::endl;
	if (listen(srvr_sckt, BACKLOG) == -1)
		std::cout << "Error: listen() failed" << std::endl;
	fcntl(srvr_sckt, F_SETFL, O_NONBLOCK);
	EV_SET(&event, srvr_sckt, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
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

//TODO make it work with the current setup
void	WebServ::runServer()
{
	int				new_evnt;
	struct kevent	*curr_evnt;
	int				clnt_fd;
	t_sckadr_strg	addr_strg;
	socklen_t		socklen;
	

	socklen = sizeof(addr_strg);
	while (true)
	{
		new_evnt = kevent(_kqueue, &_change_ev[0], _change_ev.size(), _ev_lst, MAX_EVENTS, NULL);
		if (new_evnt == -1)
			std::cout << "Error: kevent failure" << std::endl;
		_change_ev.clear();
		for (int i = 0; i < new_evnt; i++)
		{
			curr_evnt = &_ev_lst[i];
			if (curr_evnt->flags & EV_ERROR)
				std::cout << "Error: Socket got deleted" << std::endl;
			else if (curr_evnt->filter == EVFILT_READ)
			{
				receiveRequest(_ev_lst[i].ident);
			}
			else if (curr_evnt->filter == EVFILT_WRITE)
			{

			}
			if (curr_evnt->ident == (uintptr_t)_server_fd[curr_evnt->ident])
			{
				clnt_fd = accept(curr_evnt->ident, (sckadr)&addr_strg, &socklen);
				if (addConnection(clnt_fd) == 0)
					eventHandler(ADD, clnt_fd);
				else
					refuseConnection(clnt_fd);
			}
			else if (curr_evnt->flags & EV_EOF)
				deleteConnection(_ev_lst[i].ident);				
		}
	}
}
