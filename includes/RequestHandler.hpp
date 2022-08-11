#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include "Server.hpp"
# include <sstream>
# include <cctype>
# include <map>

const std::string INTERNAL_SERVER_ERROR_500 = "<!DOCTYPE html> \
<html lang='en'> \
  <head> \
    <meta charset='UTF-8'> \
    <title>500 Internal Server Error</title> \
  </head> \
  <body bgcolor='white'> \
    <center><h1>404 Not Found</h1></center> \
  </body> \
</html>";

class Server;

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
		int			getBody(void) const;
		const char	*getHeader(void) const;
		//std::string	make

		// Setters

		void	setResponse(std::string response);
		void	setRequestMsg(std::string msg);
		bool	parseFirstLine(std::string method);
		bool	isMethodImplimented(std::string line, std::string availableMethod);
		bool	isMethodFollowedBySpace(std::string line, std::string availableMethod);
		int		buildResponse(std::string);
		int		ParseRequestLine(std::string line);
		bool	fileExists(const std::string &path);
		int		OpenFile(void);
		int		ParseRequestMsg(void);
		//void		(std::string name, );

		//utils
		std::string	ltrim(const std::string &s);
		std::string	rtrim(const std::string &s);
		std::string trim(const std::string &s);

		
	private:
		Server			*_server;
		std::string		_msg;
		int				_body;
		std::string		_header;
		std::string		_method;
		std::string		_protocol;
		std::string		_uri;
};

#endif
