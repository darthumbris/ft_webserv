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
		const std::string		&getRequestBody(void) const;
		const Location			&getMatchingLocation(void) const;
		const std::string		&getRequestMethod(void) const;
		std::string				getResponse(void) const;
		bool					isRequestComplete(void) const;
		int						getFileDescriptor(void) const;
		std::size_t				getFileSize(void) const;
		t_strmap				getHeaderMap(void) const;
		t_url					getUrl(void) const;
		int						getPort(void) const;
		std::string				getUri(void) const;
		std::string				getRequestProtocol(void) const;
		std::string				getClientIp(void) const;
		const std::string&		getFileName() const;

		// Setters
		void	setClientIp(const std::string &ip);
		void	setContentType(const std::string &type);
		void	setResponseStatus(const std::string &status_line);
		void	setPort(int port);
		void	setUrlStruct(const std::string &full_url);
		void	setCompleteRequest(const std::string &request_msg);

		// Member Functions
		void	addToRequestMsg(char *msg, int bytes_received);
		void	checkRequestComplete(void);
		void	buildResponsePage(void);
		void	buildDefaultResponsePage(void);
		void	parseRequestLine(void);
		void	parseHeaderMap(void);
		void	handleRequest(void);
		void	openFile(std::string& file_to_open);
		bool	isResponseDone(void) const;
		void	findLocationForUri(void);
		void	findServer(void);
		void	buildResponseHeader(void);
		void	makeHeaderMap(void);
		void	parseResponse(void);
		void	deChunkRequestBody(void);
		void	handleGetMethod(std::string &file_to_open);
		void	handlePostMethod();
		void	handleDeleteMethod(std::string &file_to_open);
		void	checkRequestBodyConditions();
		
	private:

		t_servmap			_srv_map; //needed for finding right server
		bool				_is_request_complete;
		int					_fd;
		std::size_t			_file_size;

		std::string			_response_header;
		std::string			_response_body;

		std::string			_client_ip;
		std::string			_request_header;
		std::string			_request_protocol;
		std::string			_uri;

		std::string			_complete_request;

		t_strmap			_headermap;
		std::string			_request_body;
		std::size_t			_request_body_length;
		std::string 		_request_method;

		t_url				_url;
		int					_port;

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
