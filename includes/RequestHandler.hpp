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

//TODO make all the request handling stuff. (parsing etc)
//TODO check if remaining request stuff is actually needed
//TODO check if maybe easier to have a _response_header and _response_body or somethign similiar
//     to make it easier for the sendresponse function, or set an fd for a file if it needs to be sent?
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

		// Setters
		void	setResponse();
		void	addToRequestMsg(const std::string &msg);
		void	setSocket(int socket); //temp function for testing stuff

		// Member Functions
		
		
	private:
		Server		*_server;
		std::string	_complete_request;
		std::string	_remaining_request;
		std::string	_msg;
		std::string	_response;
		std::string	_response_body;
		bool		_is_request_complete;
		bool		_has_remaining_request;
		int			_fd;
};

#endif
