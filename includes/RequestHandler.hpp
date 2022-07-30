#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include "Server.hpp"
# include <sstream>
# include <cctype>
# include <map>


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
		
		const std::string				_protocol = "HTTP/1.1";
		const std::vector<std::string>	_availableMethods = {"GET", "POST", "DELETE"};
		std::string								_method;
		
	private:
		Server									*_server;
		std::string								_msg;
		std::string								_response;
		std::multimap<std::string, std::string>	_header;

		//const std::string[4]	_requests = "HTTP/1.1"; //maybe a map of DELETE GET POST
};

#endif
