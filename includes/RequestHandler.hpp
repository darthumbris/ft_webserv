#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include "Server.hpp"
# include <sstream>
# include <cctype>
# include <fcntl.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fstream>
# include <sstream>
# include "AutoIndexGenerator.hpp"

const std::string INTERNAL_SERVER_ERROR_500 = "<!DOCTYPE html> \
<html lang='en'> \
  <head> \
    <meta charset='UTF-8'> \
    <title>500 Internal Server Error</title> \
  </head> \
  <body bgcolor='white'> \
    <center><h1>500 Internal Server Error</h1></center> \
  </body> \
</html>";

//TODO make all the request handling stuff. (parsing etc) for alkrust
//TODO check if remaining request stuff is actually needed

class RequestHandler
{
	public:
		// Constructors
		RequestHandler(const t_servmap &srv_map);
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
		std::string	getResponse(void) const;
		bool		isRequestComplete() const;
		std::string	getRemainingRequestMsg() const;
		bool		hasRemainingRequestMsg() const;
		int			getFileDescriptor() const;
		std::size_t	getFileSize(void) const;
		void		test(void);
		Location	*getLocation(std::string url) const;

		// Setters
		void	setResponse();
		void	addToRequestMsg(const std::string &msg);
		void	setSocket(int socket); //temp function for testing stuff
		void	setPort(int port);

		
	private:
		std::string		_msg;
		int				_request_body;
		std::string		_method;
		std::string		_protocol;
		std::string		_uri;

		t_servmap	_srv_map;
		std::string	_complete_request;
		std::string	_remaining_request;
		std::string	_response_header;
		std::string	_response_body;
		bool		_is_request_complete;
		bool		_has_remaining_request;
		bool		_send_file;
		int			_client_socket;
		int			_fd_response;
		int			_port;
		std::size_t	_file_size;
};

#endif
