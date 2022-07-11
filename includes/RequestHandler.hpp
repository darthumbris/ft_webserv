#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include <iostream>
# include <string>
# include "Server.hpp"

class Server;

//TODO make all the request handling stuff. (parsing etc)
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

		// Setters
		void	setResponse();
		void	addToRequestMsg(const std::string &msg);

		// Member Functions
		
		
	private:
		Server		*_server;
		std::string	_complete_request;
		std::string	_msg;
		std::string	_response;
		int			_content_length;
		bool		_is_request_complete;
};

#endif
