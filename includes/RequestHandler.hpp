#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include "Server.hpp"
# include <sstream>
# include <cctype>
# include <map>
# include "ReadyResponses.hpp"

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
		bool	parseFirstLine(std::string method);
		bool	isMethodImplimented(std::string line, std::string availableMethod);
		bool	isMethodFollowedBySpace(std::string line, std::string availableMethod);


		// Member Functions
		// 405 An origin server SHOULD return the status code 405 (Method Not Allowed) if the method is known by the origin server but not allowed for the requested resource,
		// and 501 (Not Implemented) if the method is unrecognized or not implemented by the origin server
		
		const std::vector<std::string>			_availableMethods = {"GET", "POST", "DELETE"};
		const std::string						_protocol = "HTTP/1.1";
		std::string								_method;
		std::string								_uri;
		std::multimap<std::string, std::string>	_header;
		
	private:
		Server									*_server;
		std::string								_msg;
		std::string								_response;

		//const std::string[4]	_requests = "HTTP/1.1"; //maybe a map of DELETE GET POST
};

#endif
