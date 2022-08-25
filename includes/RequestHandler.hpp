#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

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

struct	url
{
	std::string	path;
	std::string	querry;
};

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

		const std::string	&getRequestBody(void) const;
		const Location		&getLocation(void) const;
		const std::string	&getMatchingDir(void) const;
		const Location		&getMatchingLocation(void) const;
		const std::string	&getStatusLine(void) const;
		const Server	&getServer(void) const;
		const std::string	&getRequestMethod(void) const;
		std::string		getResponse(void) const;
		int				getBody(void) const;
		const char		*getHeader(void) const;
		int				getResponseBody(void) const;
		bool			isRequestComplete(void) const;
		std::string		getRemainingRequestMsg(void) const;
		int				getFileDescriptor(void) const;
		std::size_t		getFileSize(void) const;
		t_strmap		getHeaderMap(void) const;
		t_url			getUrl(void) const;
		int				getPort(void) const;
		std::string		getCompleteRequest(void) const;
		std::string		getUri(void) const;
		const std::string	&getHost(void) const;
		std::string		getRequestProtocol(void) const;
		

		// Setters
		void	setClientIp(const std::string &ip);
		void	setMatchingDir(const std::string &matching_dir);
		void	setMatchingLocation(const Location &location);
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
		void	setServer(const Server &server);
		void	setLocations(const t_locmap &locations);
		void	addToRequestMsg(char *msg, int bytes_received);
		void	setCompeleteRequest(const std::string &request_msg);
		std::string	getClientIp(void) const;

		// Member Functions
		bool	UserHasDefinedRespnosePage(void);
		void	checkRequestComplete(void);
		void	BuildResponsePage(void);
		bool	parseFirstLine(const std::string &method);
		void	BuildDefaultResponsePage(void);
		void	ParseRequestLine(void);
		void	ParseHeaderMap(void);
		void	ParseRequestMsg(void);
		bool	fileExists(const std::string &path);
		void		OpenFile(void);

		bool	isResponseDone(void) const;
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
		void	ParseResponse(void);
		void	deChunkRequestBody(void);
		
	private:

		t_servmap			_srv_map;
		bool				_is_request_complete;
		bool				_send_file;
		bool				_cgi_error;
		int					_fd;
		std::size_t			_file_size;

		std::string			_response_header;
		std::string			_response_body;

		std::string			_client_ip;
		std::string			_request_header;
		std::string			_request_protocol;
		std::string			_uri;

		std::string			_complete_request;

		int					_client_socket;

		t_strmap			_headermap;
		std::string			_request_body;
		std::string 		_request_method; //set this, this is used for the cgihandler 

		t_url				_url;
		bool				_is_response_done;
		int					_port;

		std::size_t						_fd_length;
		bool							_is_request_header_done;
		bool							_is_chunked;

		std::string						_host;
		std::string						_status_line;

		std::string						_request_host;
		std::string						_content_type;
		std::string						_file_name;
		Server							_server;
		Location						_matching_location;
		std::string						_matching_dir;
		t_locmap						_location;
		std::string						_server_start_dir;
		std::string						_file_to_get;
		std::string						_requested_dir;
		bool							_is_folder;
}; 

#endif
