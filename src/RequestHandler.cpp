#include "RequestHandler.hpp"

/*TODO need to have a check here for post to check how big the upload is
// and compare it to the client_body_size variable of the server
*/

// Constructors
//TODO server is necesarry to check for location , accepted methods etc.
RequestHandler::RequestHandler(Server *server) : 
		_server(server), _is_request_complete(false), 
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

// Setters
void	RequestHandler::setResponse()
{
	this->_is_request_complete = true;
}


void	RequestHandler::setSocket(int socket)
{
	this->_fd = socket;
}

int sendData(int sckt, const void *data, int datalen)
{
	const char *ptr = static_cast<const char*>(data);

	while (datalen > 0)
	{
		int bytes = send(sckt, ptr, datalen, 0);
		if (bytes <=0)
			return -1;
		ptr += bytes;
		datalen -= bytes;
	}
	return 0;
}

int sendStr(int sckt, const std::string &s)
{
	return sendData(sckt, s.c_str(), s.size());
}

//TODO remove all the stuff and move it to different function (testFunction or something)
void	RequestHandler::addToRequestMsg(const std::string &msg)
{
	size_t	crlf_pos;
	size_t	size_req;

	_complete_request += msg;
	crlf_pos = _complete_request.find("\r\n\r\n");
	size_req = _complete_request.size();
	if (crlf_pos != std::string::npos)
	{
		_is_request_complete = true;
		std::string root = "var/www/html/";
		if (_complete_request.find("GET /") != std::string::npos) // testing how image things are handled
		{
			std::string path;
			if (_complete_request.find("/favicon.ico") != std::string::npos)
				path = root + "favicon.ico";
			else if (_complete_request.find("/cheese.png") != std::string::npos)
				path = root + "cheese.png";
			else if (_complete_request.find("/index.html") != std::string::npos)
				path = root + "index.html";
			else if (_complete_request.find("/dirtest.html") != std::string::npos)
				path = root + "dirtest.html";
			else if (_server->getLocationMap()["/"] && _server->getLocationMap()["/"]->getAutoIndex())
			{
				std::size_t start = _complete_request.find("GET /") + 5;
				std::size_t end = _complete_request.find("HTTP/1.1") - 1;
				std::string path = _complete_request.substr(start, end - start);
				_response_body = AutoIndexGenerator(path, root + path).getDirectoryIndex();
				std::size_t length = _response_body.length();
				std::string content_type = "text/html";
				_response_header = ("HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(length) + "\r\nConnection: keep-alive\r\nContent-Type: " + content_type + "\r\n\r\n");
				return ;
			}
			else
				path = root + "index.html";
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
				std::size_t length = infile.tellg();
				// std::cout << "length of file: " << length << std::endl;
				infile.seekg(0, std::ios::beg);
				_file_size = length;
				_response_header = ("HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(length) + "\r\nConnection: keep-alive\r\nContent-Type: " + content_type + "\r\n\r\n");
				if (infile.fail())
				{
					std::cout << "failed to get size of file" << std::endl;
					_response_header.empty();
					_response_header = "HTTP/1.1 500 Error\r\nContent-Length: 0\r\nConnection: keep-alive\r\n\r\n";
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

	//TODO make sure to parse this message. 
	//TODO might need a need to see if the msg is done being received?
	// for request headers they always end with \r\n\r\n (section 4.1 of RFC 2616)
	// We can use that to check if the full message has been received.
	// if it has been fully received set a bool _is_request_complete or something to true?
	// might also work for sending the response?
	// std::cout << "reached end of addMsg" << std::endl;
}
