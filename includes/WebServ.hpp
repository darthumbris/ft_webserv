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

# define BACKLOG 5
# define MAX_EVENTS 32
# define NUM_CLIENTS 10
# define MAX_MSG_SIZE 256

enum	event_types
{
	ADD,
	DELETE
};

class WebServ
{
	public:
		// Constructors
		WebServ(int port, char *address);
		WebServ(const WebServ &copy);
		
		// Destructor
		~WebServ();
		
		// Operators
		WebServ & operator=(const WebServ &assign);

		// Getters
		int		getConnectionId(int client_socket) const;


		// Member functions
		int		deleteConnection(int client_socket);
		void	refuseConnection(int client_socket);
		int		addConnection(int client_socket);
		void	receiveMsg(int client_socket);
		void	runServer();
		void	stopServer();
		void	eventHandler(int event_type);
		
	private:
		
		int						_srv_fd;
		int						_kqueue;
		int						_n_events;
		struct kevent			_ev_set;
		struct kevent			_ev_set_run;
		struct kevent			_ev_lst[MAX_EVENTS];
		struct sockaddr_in		_address;
		struct sockaddr_storage	_addr;
		socklen_t				_socklen;
		int						_clnt_fd;
		struct s_client_data
		{
			int	fd;
		} clients[NUM_CLIENTS];
};

#endif
