#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <fcntl.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
// # include <sys/socket.h>
// # include <netinet/in.h>

# include "Config.hpp"
# include "RequestHandler.hpp"

# define BACKLOG 		20 //not sure yet what a proper value is (most systems do 20?)
# define MAX_EVENTS 	32
# define NUM_CLIENTS 	10
# define MAX_MSG_SIZE 	256
# define MAX_FD			1024


enum	event_types
{
	ADD,
	DELETE
};

using t_sckadr = struct sockaddr;
using t_addr_in = struct sockaddr_in;
using t_event = struct kevent;
using t_ev_lst = std::vector<t_event>;
using t_evudat = struct ev_data;

struct ev_data
{
	bool			flag; // flag for deleting connection
	int				port; // port of the server
	std::string		ip; //Ip of the server
	std::string		key; // ip:port of the server
	RequestHandler	*req; // request of the client
	t_addr_in		addr; // address of the client
};

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
		
		int			_kqueue;
		int			_n_servers;
		Config		*_config;
		t_ev_lst 	_change_ev;
};

#endif
