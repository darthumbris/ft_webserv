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

struct	url
{
	std::string	path;
	std::string	querry;
};

using	t_strmap = std::map<std::string, std::string>;
using	t_url	= struct url;

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
		int		BuildResponse(std::string);
		int		ParseRequestLine(std::string line);
		bool	fileExists(const std::string &path);
		int		OpenFile(void);
		int		ParseRequestMsg(void);

		//utils
		std::string	ltrim(const std::string &s);
		std::string	rtrim(const std::string &s);
		std::string trim(const std::string &s);
		int			getResponseBody(void) const;
		bool		isRequestComplete() const;
		std::string	getRemainingRequestMsg() const;
		bool		hasRemainingRequestMsg() const;
		int			getFileDescriptor() const;
		std::size_t	getFileSize(void) const;
		void		test(void);
		Location	*getLocation(std::string url) const;
		std::string	getRequestMethod() const;
		t_strmap	getHeaderMap() const;
		t_url		getUrl() const;
		int			getPort() const;
		std::string	getCompleteRequest() const;
		std::string&	getRequestBody();

		// Setters
		void	setCgiError(void);
		void	setResponse();
		void	addToRequestMsg(char *msg, int bytes_received);
		void	setSocket(int socket);
		void	setPort(int port);
		void	setUrlStruct(std::string full_url);
		void	setClientIp(std::string ip);
		std::string	getClientIp(void) const;

		// Member Functions
		void	makeHeaderMap();
		void	testFunction();
		
	private:
		t_servmap		_srv_map;
		bool			_is_request_complete;
		bool			_has_remaining_request;
		bool			_send_file;
		int				_fd_response;
		int				_response_body;
		std::size_t		_file_size;

		std::string		_client_ip;
		std::string		_msg;
		std::string		_request_header;
		std::string		_method;
		std::string		_protocol;
		std::string		_uri;

		std::string		_complete_request;
		std::string		_remaining_request;
		std::string		_response_header;

		int				_client_socket;
		std::size_t		_fd_length;

		t_strmap		_headermap;
		std::string		_request_body;
		std::string 	_request_method; //set this, this is used for the cgihandler 

		t_url			_url;
		bool			_is_request_header_done;
		bool			_cgi_error;
		int				_port;
};

#endif
