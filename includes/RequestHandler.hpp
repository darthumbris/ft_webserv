#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "Server.hpp"
# include <fcntl.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fstream>
# include <sstream>
# include "AutoIndexGenerator.hpp"

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
		std::string	getResponse() const;
		bool		isRequestComplete() const;
		std::string	getRemainingRequestMsg() const;
		bool		hasRemainingRequestMsg() const;
		int			getFileDescriptor() const;
		std::size_t	getFileSize() const;
		Location	*getLocation(std::string url) const;
		std::string	getRequestMethod() const;
		std::string	getResponseBody() const;
		t_strmap	getHeaderMap() const;
		t_url		getUrl() const;
		int			getPort() const;
		std::string	getClientIp() const;

		// Setters
		void	setResponse();
		void	addToRequestMsg(const std::string &msg);
		void	setSocket(int socket); //temp function for testing stuff
		void	setPort(int port);
		void	setUrlStruct(std::string full_url);
		void	setClientIp(std::string ip);

		// Member Functions
		void	makeHeaderMap();
		
	private:

		t_servmap	_srv_map;
		std::string	_complete_request;
		std::string	_remaining_request;
		std::string	_response_header;
		t_strmap	_headermap;
		std::string	_response_body;
		std::string	_request_body;
		std::string _request_method; //set this, this is used for the cgihandler 
		std::string	_method_header;
		std::string	_client_ip;

		t_url		_url;
		bool		_is_request_complete;
		bool		_has_remaining_request;
		bool		_send_file;
		int			_client_socket;
		int			_fd;
		int			_port;
		std::size_t	_file_size;
};

#endif
