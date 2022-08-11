#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include "Server.hpp"
# include <sstream>
# include <cctype>
# include <map>

const char std::string = "<!DOCTYPE html> \
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
		std::string	getContent() const;
		std::string	getHeader() const;
		//std::string	make

		// Setters

		void		setResponse(std::string response);
		void		setRequestMsg(std::string msg);
		bool		parseFirstLine(std::string method);
		bool		isMethodImplimented(std::string line, std::string availableMethod);
		bool		isMethodFollowedBySpace(std::string line, std::string availableMethod);
		void		buildResponse(std::string);
		void		ParseRequestLine(std::string line);
		bool		fileExists(const std::string &path);
		//void		(std::string name, );

		//utils
		bool		openFile(const char *path, int mode);
		std::string	ltrim(const std::string &s);
		std::string	rtrim(const std::string &s);
		std::string trim(const std::string &s);

		
	private:
		Server									*_server;
		std::string								_msg;
		int										_body;
		char									*_header;
		std::string											_method;
		std::string											_uri;
		std::vector<std::pair<std::string, std::string>>	_tmpHeader;
};

#endif
