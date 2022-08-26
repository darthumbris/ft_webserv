#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <fcntl.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>

# include "Config.hpp"
# include "RequestHandler.hpp"
# include "AutoIndexGenerator.hpp"

# define BACKLOG 		10000 //not sure yet what a proper value is (most systems do 20?)
# define MAX_EVENTS 	1024
# define NUM_CLIENTS 	1024
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
	int				flag; // flag for deleting connection
	int				port; // port of the server
	std::string		ip; //Ip of the server
	RequestHandler	*req; // request of the client
	t_addr_in		addr; // address of the client
	off_t			datalen; // used for sending response
	size_t			total_size; // used for sending response
};

class WebServ
{
	public:
		// Constructors
		WebServ(t_servmap& servers);
		// WebServ(int port, char *address);
		WebServ(const WebServ &copy);
		
		// Destructor
		~WebServ();
		
		// Operators
		WebServ &operator=(const WebServ &assign);

		// Getters
		bool		listeningToPort(int port) const;
		t_servmap	getServers() const;
	
		// Setters
		void	addPortToList(int port);

		// Member functions
		void	setNewServerSocket(Server server, int port);
		bool	isListenSocket(int fd);
		void	addConnection(t_event event, t_evudat *old_udat);
		void	readFromSocket(t_event &event);
		void	writeToSocket(t_event &event);
		void	deleteConnection(t_event event, int16_t	filter);
		void	receiveRequest(t_event &event);
		void	sendResponse(t_event &event);
		void	runServer(void);

		//Exception
		class WebServerExcpetion: public std::exception {
		private:
			std::string _msg;
		public:
			WebServerExcpetion(const std::string& msg) : _msg(msg) {}
			virtual const char* what() const throw() {
				return _msg.c_str();
			}
		};
		
	private:
		
		int			_kqueue;
		int			_n_servers;
		t_vecint	_ports;
		t_servmap	_servers;
		t_ev_lst 	_change_ev;
};

#endif
