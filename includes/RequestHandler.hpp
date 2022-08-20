#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

//includes iostream vector string map
# include <sstream>
# include <cctype>
# include <fcntl.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fstream>
# include <sstream>
# include "Server.hpp"
# include "AutoIndexGenerator.hpp"

/*
const std::string &INTERNAL_SERVER_ERROR_500 = "<!DOCTYPE html> \
<html lang='en'> \
  <head> \
    <meta charset='UTF-8'> \
    <title>500 Internal Server Error</title> \
  </head> \
  <body bgcolor='white'> \
    <center><h1>500 Internal Server Error</h1></center> \
  </body> \
</html>";
*/

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

		std::string	getResponse(void);
		int					getBody(void) const;
		const char			*getHeader(void) const;
		//std::string	make

		bool	parseFirstLine(const std::string &method);
		void	ParseRequestLine(void);
		int		ParseHeaderMap(void);
		int		ParseRequestMsg(void);
		bool	fileExists(const std::string &path);
		void		OpenFile(void);

		int			getResponseBody(void) const;
		bool		isRequestComplete() const;
		std::string	getRemainingRequestMsg() const;
		bool		hasRemainingRequestMsg() const;
		int			getFileDescriptor() const;
		std::size_t	getFileSize(void) const;
		void		test(void);
		//Location	*getLocation(const std::string &url) const;
		std::string	getRequestMethod() const;
		t_strmap	getHeaderMap() const;
		t_url		getUrl() const;
		int			getPort() const;
		std::string	getCompleteRequest() const;
		std::string&	getRequestBody();

		// Setters
		void	setRequestMsg(const std::string &msg);
		void	setContentType(const std::string &type);
		void	setResponseStatus(const std::string &status_line);
		void	setFdBody(int fd);
		void	setFileName(const std::string &name);
		void	setDefaultHost(const std::string &name);
		void	setDefaultServer(const Server *server);
		void	setHost(const std::string &name);
		void	setCgiError(void);
		void	setResponseCompelete(const bool &status);
		void	setSocket(int socket);
		void	setPort(int port);
		void	setUrlStruct(const std::string &full_url);
		void	setClientIp(const std::string &ip);
		void	setServer(const Server &server);
		void	setLocations(const t_locmap &locations);
		void	setLocation(const Location &location);
		void	addToRequestMsg(char *msg, int bytes_received);
		std::string	getClientIp(void) const;

		// Member Functions
		void	FindTheRightLocationForUri(void);
		//void	FindLocationPath(void);
		void	FindAllAccessibleLocations(void);
		int		CheckUserDefinedStatusPage(Server server);
		void	FindServer(void);
		//void	FindBestFithLocation(void);
		void	BuildDefaultResponseBody(void);
		void	BuildResponseHeader(void);
		void	makeHeaderMap(void);
		void	testFunction(void);
		
	private:
		t_servmap		_srv_map;
		bool			_is_request_complete;
		bool			_has_remaining_request;
		bool			_send_file;
		int				_fd_response;
		std::size_t		_file_size;

		std::string		_client_ip;
		std::string		_msg;
		std::string		_request_header;
		std::string		_protocol;
		std::string		_uri;

		std::string		_complete_request;
		std::string		_remaining_request;
		std::string		_response_header;

		int				_client_socket;

		t_strmap		_headermap;
		std::string		_request_body;
		std::string 	_request_method; //set this, this is used for the cgihandler 

		t_url			_url;
		bool			_is_request_header_done;
		bool			_cgi_error;
		int				_port;

		std::size_t						_fd_length;
		std::string						_host;
		std::string						_default_host;
		std::string						_content_type;
		std::string						_file_name;
		std::string						_status_line;
		Location						_location;
		Server							_server;
		t_locmap						_locations;
}; 
#endif
