#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "Server.hpp"
# include <fcntl.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fstream>
# include <sstream>
# include "AutoIndexGenerator.hpp"

//TODO make all the request handling stuff. (parsing etc) for alkrust
//TODO check if remaining request stuff is actually needed
//TODO maybe instead of adding a file to response_body instead set the fd and use sendfile in the webserv
class RequestHandler
{
	public:
		// Constructors
		RequestHandler(Server *server);
		RequestHandler(const RequestHandler &copy);
		
		// Destructor
		~RequestHandler();
		
		// Operators
		RequestHandler & operator=(const RequestHandler &assign);

		// Getters
		std::string	getResponse() const;
		bool		isRequestComplete() const;
		std::string	getRemainingRequestMsg() const;
		bool		hasRemainingRequestMsg() const;
		int			getFileDescriptor() const;
		std::size_t	getFileSize() const;

		// Setters
		void	setResponse();
		void	addToRequestMsg(const std::string &msg);
		void	setSocket(int socket); //temp function for testing stuff

		// Member Functions
		
		
	private:
		Server		*_server;
		std::string	_complete_request;
		std::string	_remaining_request;
		std::string	_response_header;
		std::string	_response_body;
		bool		_is_request_complete;
		bool		_has_remaining_request;
		bool		_send_file;
		int			_fd;
		std::size_t	_file_size;
};

#endif
