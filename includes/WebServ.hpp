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

# define BACKLOG 		10000
# define MAX_EVENTS 	32
# define NUM_CLIENTS 	10
# define MAX_MSG_SIZE 	256
# define MAX_FD			1024

enum	event_types
{
	ADD,
	DELETE
};

typedef struct sockaddr 		*sckadr;
typedef struct sockaddr_storage	t_sckadr_strg;

typedef struct ev_data
{
	int					flag;
	struct sockaddr_in	addr;
	std::string			ip;
	int					port;
}	t_evudat;

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

		// Setters
		void	setNewServerSocket(Server *server);

		// Member functions
		bool	isListenSocket(int fd);
		void	addConnection(struct kevent event);
		void	readSocket(struct kevent &event);
		void	deleteConnection(struct kevent event);
		void	receiveRequest(int client_socket);
		void	runServer();
		
	private:
		
		int							_kqueue;
		int							_n_servers;
		Config						*config;
		std::vector<struct kevent> _change_ev;
};

#endif
