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
# include <map>
# include "Config.hpp"
# include "RequestHandler.hpp"

# define BACKLOG 		10000
# define MAX_EVENTS 	32
# define NUM_CLIENTS 	10
# define MAX_MSG_SIZE 	256
# define MAX_FD			1024

class Config;
class RequestHandler;

enum	event_types
{
	ADD,
	DELETE
};

typedef struct sockaddr 		*sckadr;
typedef struct sockaddr_in		t_addr_in;
typedef struct kevent			t_event;

typedef struct ev_data
{
	int				flag;
	int				port;
	std::string		ip;
	std::string		key;
	RequestHandler	*req;
	t_addr_in		addr;
}	t_evudat;

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
		Server	*getServer(std::string key) const;
		// Setters

		// Member functions
		void	setNewServerSocket(Server *server);
		bool	isListenSocket(int fd);
		void	addConnection(t_event event, t_evudat *old_udat);
		void	readFromSocket(t_event &event);
		void	writeToSocket(t_event &event);
		void	deleteConnection(t_event event, int16_t	filter);
		void	receiveRequest(t_event &event);
		void	sendResponse(t_event &event);
		void	runServer();
		
	private:
		
		int							_kqueue;
		int							_n_servers;
		Config						*_config;
		std::vector<t_event> _change_ev;
};

#endif
