/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 11:01:06 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/12 12:51:58 by alkrusts      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include <fcntl.h>
#include <fstream>
#include <unistd.h> //for access

//A server SHOULD return 414 (Request-URI Too Long) status if a URI is longer than the server can handle (see section 10.4.15).

const std::string WHITESPACE = " \n\r\t\f\v";

/*
*/

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

bool RequestHandler::fileExists(const std::string& path)
{
	std::ifstream fstream(path);

	return (fstream.good());
}

/*
bool RequestHandler::filePremissions(const std::string& path)
{
	std::filesystem::premissions(path);

	return (fstream.good());
}
*/

// Constructors
//TODO server is necesarry to check for location , accepted methods etc.
RequestHandler::RequestHandler(const t_servmap &srv_map) : 
		_srv_map(srv_map), _is_request_complete(false), 
		_has_remaining_request(false), _send_file(false), _fd_response(0),
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

std::vector<std::string>	cpp_split(std::string line)
{
	std::size_t 				prev = 0, pos;
	std::vector<std::string>	wordVector;

	while ((pos = line.find_first_of(" ", prev)) != std::string::npos)
	{
		if (pos > prev)
			wordVector.push_back(trim(line.substr(prev, pos - prev)));
		prev = pos + 1;
	}
	if (prev < line.length())
		wordVector.push_back(trim(line.substr(prev, std::string::npos)));
	return (wordVector);
}

//Getters

const char	*RequestHandler::getHeader(void) const
{
	return (_response_header.c_str());
}

int			RequestHandler::getBody(void) const
{
	return (_fd_response);
}

int	RequestHandler::buildResponse(std::string request)
{
	if (request == "INTERNAL SERVER ERROR 500")
	{
		_response_header += "HTTP/1.1 Internal Server Error 500\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Length: " + std::to_string(INTERNAL_SERVER_ERROR_500.length()) + "\r\n";
		_response_header += "Content-Type: text/html\r\n\r\n";
		_response_header += INTERNAL_SERVER_ERROR_500;
	}
	/*
	 * CLIENT ERROR 4xx
	 */
	if (request == "400 BAD REQUEST")
	{
		std::ifstream infile("var/www/400.html", std::ios::in);
		if (infile.fail())
			return buildResponse("INTERNAL SERVER ERROR 500");
		infile.seekg(0, std::ios::end);
		std::size_t length = infile.tellg();
		infile.seekg(0, std::ios::beg);
		infile.close();
		_fd_response = open("var/www/400.html", O_RDONLY);
		if (_fd_response == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Bad Request 400\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Length: " + std::to_string(length) + "\r\n";
		_response_header += "Content-Type: text/html\r\n\r\n";
	}
	if (request == "UNAUTHORIZED 401")
	{
		_fd_response = open("var/www/401.html", O_RDONLY);
		if (_fd_response == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Bad Request 401\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}
	if (request == "PAYMENT REQUIRED 402") //this code is reserver for future use XD rfc2626
	{
		_fd_response = open("var/www/402.html", O_RDONLY);
		if (_fd_response == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Payment Required 402\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}
	if (request == "NOT FOUND 403")
	{
		_fd_response = open("var/www/403.html", O_RDONLY);
		if (_fd_response == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Not Found 404\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}	
	if (request == "NOT FOUND 404")
	{
		_fd_response = open("var/www/404.html", O_RDONLY);
		if (_fd_response == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Not Found 404\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}
	if (request == "BAD REQUEST 400")
	{
		_fd_response = open("var/www/400.html", O_RDONLY);
		if (_fd_response == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Bad Request 400\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}
	_is_request_complete = true;
	return (1);
}

int	RequestHandler::ParseRequestLine(std::string line)
{
	const std::vector<std::string>	wordVector = cpp_split(line);

	if (wordVector.size() != 3)
		return buildResponse("400 BAD REQUEST");
	if (wordVector[0] != "GET" && wordVector[0] != "DELETE" && wordVector[0] != "POST")
		return buildResponse("400 BAD REQUEST");
	if (wordVector[1][0] != '/')
		return buildResponse("400 BAD REQUEST");
	if (wordVector[2] != "HTTP/1.1")
		return buildResponse("400 BAD REQUEST");
	_method = wordVector[0];
	_uri = wordVector[1];
	_protocol = wordVector[2];
	return (0);
}

int	RequestHandler::OpenFile(void)
{
	int	status;

	status = access(_uri.c_str(), F_OK); //checks if file exists
	if (status == -1)
		return buildResponse("404 NOT FOUND");
	if ("GET" == _method)
	{
		status = access(_uri.c_str(), R_OK); //checks if file is redable
		if (status == -1)
			return buildResponse("403 PERMISSION DENIED");
	}
	if ("POST" == _method)
	{
		status = access(_uri.c_str(), W_OK); //checks if file is writeble
		if (status == -1)
			return buildResponse("403 PERMISSION DENIED");
	}
	if ("DELETE" == _method)
	{
	}
	return (0);
}

//wrap this on in try catch block if this fails return internal server error :D I fucking h8 c++
//asembly or die
void	RequestHandler::setRequestMsg(std::string msg) 
{
	_msg = msg;
}

int	RequestHandler::ParseRequestMsg(void)
{
	std::istringstream iss;

	iss.str(_msg);
	std::string	line;
	std::getline(iss, line);
	if (ParseRequestLine(line)
		|| OpenFile())
		return (1);
	while (1)
	{
		std::getline(iss, line);
		if (iss.eof())
			break;
		if (iss.fail())
			return buildResponse("INTERNAL SERVER ERROR 500");
	}
	return (0);
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
	return this->_fd_response;
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
	this->_client_socket = socket;
}

void	RequestHandler::setPort(int port)
{
	this->_port = port;
}

std::string	RequestHandler::getResponse(void) const
{
	return (_response_header + _response_body);
}

// Member functions
Location	*RequestHandler::getLocation(std::string url) const
{
	for (std::size_t it = 0; it < _srv_map.size(); it++)
	{
		if (_srv_map[it]->getLocation(_port, url))
			return (_srv_map[it]->getLocation(_port, url));
	}
	return NULL;
}
void	RequestHandler::test(void)
{
	std::string root = "var/www/html/";
	std::string	url = "/";
	std::string path;
	Location *loc;
	if (_complete_request.find("/favicon.ico") != std::string::npos)
		path = root + "favicon.ico";
	else if (_complete_request.find("/cheese.png") != std::string::npos)
		path = root + "cheese.png";
	else if (_complete_request.find("/index.html") != std::string::npos)
		path = root + "index.html";
	else if (_complete_request.find("/dirtest.html") != std::string::npos)
		path = root + "dirtest.html";
	else if ((loc = this->getLocation(url)))
	{
		if (!loc->getAutoIndex())
			return;
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
			_fd_response = open(path.c_str(), O_RDONLY);
		}
	}
}

//TODO remove all the stuff and move it to different function (testFunction or something)
void	RequestHandler::addToRequestMsg(const std::string &msg)
{
	size_t	crlf_pos;
	size_t	size_req;

	_complete_request += msg;
	crlf_pos = _complete_request.find("\r\n\r\n");
	std::cout << "Important: " << crlf_pos << std::endl;
	//return buildResponse("INTERNAL SERVER ERROR 500");
	size_req = _complete_request.size();
	if (crlf_pos != std::string::npos)
	{
		setRequestMsg(_complete_request);
		ParseRequestMsg();
	}
}
