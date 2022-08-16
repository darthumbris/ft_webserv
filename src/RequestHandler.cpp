#include "RequestHandler.hpp"
#include "CgiHandler.hpp"

/*TODO need to have a check here for post to check how big the upload is
// and compare it to the client_body_size variable of the server
*/

// Constructors
//TODO server is necesarry to check for location , accepted methods etc.
RequestHandler::RequestHandler(const t_servmap &srv_map) : 
		_srv_map(srv_map), _is_request_complete(false), 
		_has_remaining_request(false), _send_file(false), _fd(0),
		_file_size(0)
{
}

RequestHandler::RequestHandler(const RequestHandler &copy)
{
	(void) copy;
}


// Destructor
RequestHandler::~RequestHandler()
{
}


// Operators
RequestHandler & RequestHandler::operator=(const RequestHandler &assign)
{
	(void) assign;
	return *this;
}

// Getters
std::string	RequestHandler::getResponse() const
{
	return this->_response_header + this->_response_body;
}

bool	RequestHandler::isRequestComplete() const
{
	return this->_is_request_complete;
}

std::string	RequestHandler::getRemainingRequestMsg() const
{
	return this->_remaining_request;
}

bool	RequestHandler::hasRemainingRequestMsg() const
{
	return this->_has_remaining_request;
}

int	RequestHandler::getFileDescriptor() const
{
	return this->_fd;
}

std::size_t	RequestHandler::getFileSize() const
{
	return this->_file_size;
}

std::string	RequestHandler::getRequestMethod() const
{
	return this->_request_method;
}

std::string RequestHandler::getResponseBody() const
{
	return this->_response_body;
}

t_strmap	RequestHandler::getHeaderMap() const
{
	return this->_headermap;
}

t_url	RequestHandler::getUrl() const
{
	return this->_url;
}

int	RequestHandler::getPort() const
{
	return this->_port;
}

std::string	RequestHandler::getClientIp() const
{
	return this->_client_ip;
}

std::string RequestHandler::getCompleteRequest() const
{
	return this->_complete_request;
}

// Setters
void	RequestHandler::setResponse()
{
	this->_is_request_complete = true;
}


void	RequestHandler::setSocket(int socket)
{
	this->_client_socket = socket;
}

void	RequestHandler::setPort(int port)
{
	this->_port = port;
}

Location	*RequestHandler::getLocation(std::string url) const
{
	for (std::size_t it = 0; it < _srv_map.size(); it++)
	{
		if (_srv_map[it].getLocation(_port, url))
			return (_srv_map[it].getLocation(_port, url));
	}
	return NULL;
}

void	RequestHandler::setUrlStruct(std::string full_url)
{
	std::size_t	q_pos = full_url.find('?');
	if (q_pos == std::string::npos)
		q_pos = full_url.length();
	_url.path = full_url.substr(0, q_pos);
	_url.querry = full_url.substr(q_pos, full_url.length());
}

void	RequestHandler::setClientIp(std::string ip)
{
	_client_ip = ip;
}

// Member functions
void	RequestHandler::makeHeaderMap()
{
	std::vector<std::string>	split;
	std::size_t	pos;
	std::size_t	last_pos;
	std::size_t	len;

	//splitting the request on \r\n
	len = _complete_request.length();
	last_pos = 0;
	while (last_pos < len + 1)
	{
		pos = _complete_request.find_first_of("\r\n", last_pos);
		if (pos == std::string::npos)
			pos = len;
		if (pos != last_pos)
			split.push_back(std::string(_complete_request.data() + last_pos, pos - last_pos));
		last_pos = pos + 1;
	}
	std::size_t	first_pos = _complete_request.find_first_of(' ') + 1;
	std::size_t	end_pos = _complete_request.find_first_of(' ', first_pos + 1);
	setUrlStruct(_complete_request.substr(first_pos, end_pos - first_pos));
	_request_method = _complete_request.substr(0, first_pos - 1);
	_method_header = _complete_request.substr(0, _complete_request.find_first_of("\r\n"));

	//making a map of all the request headers
	if (split.size() > 1)
	{
		for (std::size_t i = 1; i < split.size(); i++)
		{
			if (split[i] == "\r\n")
				break ;
			pos = split[i].find_first_of(':');
			_headermap[split[i].substr(0, pos)] = split[i].substr(pos + 2, split[i].length());
		}
	}
}

// 3 functions below this just for a bit of testing stuff
#include <sys/stat.h>
#include <mach-o/dyld.h>

enum	path
{
	IS_DIR,
	IS_FILE,
	IS_OTHER
};

static int	checkPath(std::string path, std::string root)
{
	struct stat s;

	unsigned int bufferSize = 512;
	std::vector<char> buffer(bufferSize + 1);

	if(_NSGetExecutablePath(&buffer[0], &bufferSize))
	{
		buffer.resize(bufferSize);
		_NSGetExecutablePath(&buffer[0], &bufferSize);
	}
	std::string cur_dir = &buffer[0];
	cur_dir.resize(cur_dir.length() - 9);
	std::string	total_path = cur_dir + root + path;

	if (stat(total_path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			std::cout << "is a directory" << std::endl;
			return IS_DIR;
		}
		else if (s.st_mode & S_IFREG)
		{
			std::cout << "is a file" << std::endl;
			return IS_FILE;
		}
		else
		{
			std::cout << "is something else" << std::endl;
			return IS_OTHER;
		}
	}
	std::cout << total_path << " is not a file or directory" << std::endl;
	return IS_OTHER;
}

static void	setServerError(std::string *body, std::string *header)
{
	*body = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n";
	*body += "<title>500 Internal Server Error</title>\n</head>\n<body bgcolor=\"white\">";
	*body += "<center><h1>500 Internal Server Error</h1></center>\n</body>\n</html>";
	*header = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: ";
	*header += std::to_string((*body).length());
	*header += "\r\nConnection: keep-alive\r\nContent-Type: ";
	*header += "text/html\r\n\r\n";
}

void	RequestHandler::testFunction()
{
	makeHeaderMap();
	
	std::cout << "urlpath: " << _url.path << std::endl;
	std::string	file = "";
	std::string url = _url.path.substr(0, _url.path.find_last_of('/') + 1);
	std::string root;
	if (getLocation(url))
		root = getLocation(url)->getRootPath();
	else
		return (setServerError(&_response_body, &_response_header));
	// root.append("/");
	if (checkPath(_url.path, root) == IS_FILE)
		file += _url.path.substr(_url.path.find_last_of('/') + 1, _url.path.length());
	else
		url = _url.path;
	if (url.length() > 1 && checkPath(_url.path, root) == IS_DIR)
		url += "/";	
	std::cout << "root: " << root << std::endl;
	std::cout << "url: " << url << std::endl;
	std::cout << "file: " << file << std::endl;
	if (_complete_request.find("GET /") != std::string::npos || _complete_request.find("POST /") != std::string::npos) // testing how image things are handled
	{
		if (_url.path.find(".php") != std::string::npos)
		{
			CgiHandler	cgi(*this);
			_response_body = cgi.execute();
			_response_body.append("\r\n\r\n");
			//TODO response header needs to be properly made based on the response body?
			_response_header = "HTTP/1.1 200 OK\r\nContent-Length: ";
			_response_header += std::to_string(_response_body.length());
			_response_header += "\r\nConnection: keep-alive\r\nContent-Type: ";
			_response_header += "Content-type: text/html; charset=UTF-8\r\n\r\n";
			return;
		}
		std::string path;
		Location *loc;
		if (file != "")
			path = root + url + file;
		else if ((loc = this->getLocation(url)) && loc->getIndex() == "")
		{
			std::cout << "auto indexing" << std::endl;
			if (!loc->getAutoIndex())
				return (setServerError(&_response_body, &_response_header));
			_response_body = AutoIndexGenerator(url, root + url).getDirectoryIndex();
			_response_header = "HTTP/1.1 200 OK\r\nContent-Length: ";
			_response_header += std::to_string(_response_body.length());
			_response_header += "\r\nConnection: keep-alive\r\nContent-Type: text/html\r\n\r\n";
			return ;
		}
		else if (loc)
			path = root + url + loc->getIndex();
		else
			return (setServerError(&_response_body, &_response_header));
		std::cout << "\n\n=======Path:" << path << std::endl;
		std::ifstream infile(path, std::ios::in);
		if (!infile.is_open())
		{
			std::cout << "failed to open file" << std::endl;
			_response_header = "HTTP/1.1 500 Error\r\nContent-Length: 0\r\nConnection: keep-alive\r\n\r\n";
		}
		else
		{
			std::string content_type;
			if (_complete_request.find("/favicon.ico") != std::string::npos)
				content_type = "image/x-icon";
			else if (_complete_request.find("/cheese.png") != std::string::npos)
				content_type = "image/png";
			else
				content_type = "text/html";
			infile.seekg(0, std::ios::end);
			std::size_t length  = infile.tellg();
			infile.seekg(0, std::ios::beg);
			_file_size = length;
			_response_header = ("HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(length) + "\r\nConnection: keep-alive\r\nContent-Type: " + content_type + "\r\n\r\n");
			if (infile.fail())
			{
				std::cout << "failed to get size of file" << std::endl;
				return (setServerError(&_response_body, &_response_header));
			}
			else if (length > 0)
			{
				_send_file = true;
				infile.close();
				_fd = open(path.c_str(), O_RDONLY);
			}
		}
	}
}

//TODO if there is a POST request it might have a body. Need to save that too for cgi handler
void	RequestHandler::addToRequestMsg(char *msg, int bytes_received)
{
	size_t	crlf_pos;

	_complete_request.append(msg, bytes_received);
	if (!isprint(_complete_request[0]))
	{
		_is_request_complete = true;
		std::cout << "bad request" << std::endl;
		return (setServerError(&_response_body, &_response_header));
	}
	crlf_pos = _complete_request.find("\r\n\r\n");
	if (crlf_pos != std::string::npos) //TODO might need a need to see if the msg is done being received? Especially with POST requests
	{
		std::cout << _complete_request << std::endl;
		std::cout << "\n------end of complete request--------" << std::endl;
		_is_request_complete = true;
		testFunction();
	}
}
