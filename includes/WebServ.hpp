#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <fcntl.h>
# include <sys/event.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <vector>
# include "Config.hpp"
# include <map>

# define BACKLOG 10000
# define MAX_EVENTS 32
# define NUM_CLIENTS 10
# define MAX_MSG_SIZE 256

enum	event_types
{
	ADD,
	DELETE
};

typedef struct sockaddr 		*sckadr;
typedef struct sockaddr_storage	t_sckadr_strg;

class Config;

class WebServ
{
	public:
		// Constructors
		WebServ(Config *config);
		WebServ(int port, char *address);
		WebServ(const WebServ &copy);
		
		// Destructor
		~WebServ();
		
		// Operators
		WebServ &operator=(const WebServ &assign);

		// Getters
		int		getConnectionId(int client_socket) const;

		// Setters
		void	setNewServerSocket(Server *server);

		// Member functions
		int		deleteConnection(int client_socket);
		void	refuseConnection(int client_socket);
		int		addConnection(int client_socket);
		void	receiveRequest(int client_socket);
		void	runServer();
		void	stopServer();
		void	eventHandler(int event_type, int client_socket);
		
	private:
		
		int							_kqueue;
		struct kevent				_ev_lst[MAX_EVENTS];
		std::vector<int>			_clients;
		Config						*config;
		std::vector<struct kevent> _change_ev;
		std::vector<int>			_server_fd;
};

#endif
