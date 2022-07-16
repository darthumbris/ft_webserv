#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include "Server.hpp"
# include <sstream>
# include <cctype>

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
		//std::string	make

		// Setters
		void	setResponse(std::string response);
		void	setRequestMsg(std::string msg);

		// Member Functions
		
		
	private:
		Server					*_server;
		const std::string		_protocol = "HTTP/1.1";
		const std::string		_methods[3] = {"GET", "POST", "DELETE"};
		//const std::string[4]	_requests = "HTTP/1.1"; //maybe a map of DELETE GET POST
		std::string				_msg;
		std::string				_response;
};

#endif
