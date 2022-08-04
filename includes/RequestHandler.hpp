#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include "Server.hpp"
# include <sstream>
# include <cctype>
# include <map>

class Server;

typedef struct s_response
{
	int			fd;
	std::string	header;
}				t_response;

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
		t_response	getResponse() const;
		//std::string	make

		// Setters
	
		void		setResponse(std::string response);
		void		setRequestMsg(std::string msg);
		bool		parseFirstLine(std::string method);
		bool		isMethodImplimented(std::string line, std::string availableMethod);
		bool		isMethodFollowedBySpace(std::string line, std::string availableMethod);
		void		(std::string name, );


		const std::vector<std::string>			_availableMethods = {"GET", "POST", "DELETE"};
		const std::string						_protocol = "HTTP/1.1";
		std::string								_method;
		std::string								_uri;
		std::multimap<std::string, std::string>	_header;
		
	private:
		Server									*_server;
		std::string								_msg;
		t_response								_response;
};

#endif
