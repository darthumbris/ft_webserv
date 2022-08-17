/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 11:01:06 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/17 19:00:23 by alkrusts      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "CgiHandler.hpp"
#include "Utils.hpp"

#include <fcntl.h>
#include <fstream>
#include <unistd.h>

//A server SHOULD return 414 (Request-URI Too Long) status if a URI is longer than the server can handle (see section 10.4.15).

const std::string WHITESPACE = " \n\r\t\f\v";

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

// Constructors
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

int	RequestHandler::BuildResponse(std::string request)
{
	/*
	 * 2xx: Success 
	 */
	if (request == "200 OK")
	{
		_response_header += "HTTP/1.1 200 OK\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Length: " + std::to_string(_fd_length) + "\r\n";
		_response_header += "Content-Type: text/html\r\n\r\n";
	}
	/*
	 * CLIENT ERROR 4xx
	 */
	else if (request == "400 BAD REQUEST")
	{
		std::ifstream infile("var/www/400.html", std::ios::in);
		if (infile.fail())
			return BuildResponse("INTERNAL SERVER ERROR 500");
		infile.seekg(0, std::ios::end);
		std::size_t length = infile.tellg();
		infile.seekg(0, std::ios::beg);
		infile.close();
		_fd_response = open("var/www/400.html", O_RDONLY);
		if (_fd_response == -1)
			return BuildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Bad Request 400\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Length: " + std::to_string(length) + "\r\n";
		_response_header += "Content-Type: text/html\r\n\r\n";
	}
	else if (request == "401 UNAUTHORIZED")
	{
		_fd_response = open("var/www/401.html", O_RDONLY);
		if (_fd_response == -1)
			return BuildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Bad Request 401\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}
	else if (request == "402 PAYMENT REQUIRED") //this code is reserver for future use XD rfc2626
	{
		_fd_response = open("var/www/402.html", O_RDONLY);
		if (_fd_response == -1)
			return BuildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Payment Required 402\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}
	else if (request == "403 NOT FOUND")
	{
		_fd_response = open("var/www/403.html", O_RDONLY);
		if (_fd_response == -1)
			return BuildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Not Found 404\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}	
	else if (request == "404 NOT FOUND")
	{
		_fd_response = open("var/www/404.html", O_RDONLY);
		if (_fd_response == -1)
			return BuildResponse("INTERNAL SERVER ERROR 500");
		_response_header += "HTTP/1.1 Not Found 404\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Type: text/html\r\n";
	}
	/*
	 * SERVER ERROR 5xx
	 */
	else if (request ==  "500 INTERNAL SERVER ERROR")
	{
		_response_header += "HTTP/1.1 Internal Server Error 500\r\n";
		_response_header += "Server: ft_webserver\r\n";
		_response_header += "Content-Length: " + std::to_string(INTERNAL_SERVER_ERROR_500.length()) + "\r\n";
		_response_header += "Content-Type: text/html\r\n\r\n";
		_response_header += INTERNAL_SERVER_ERROR_500;
	}
	_is_request_complete = true;
	return (1);
}

int	RequestHandler::ParseRequestLine(std::string line)
{
	const std::vector<std::string>	wordVector = cpp_split(line);

	//hex for space %20
	if (wordVector.size() != 3)
		return BuildResponse("400 BAD REQUEST");
	if (wordVector[0] != "GET" && wordVector[0] != "DELETE" && wordVector[0] != "POST")
		return BuildResponse("400 BAD REQUEST");
	// Special characters ! * ' ( ) ; : @ & = + $ , / ? % # [ ] 
	if (wordVector[1][0] != '/')
		return BuildResponse("400 BAD REQUEST");
	if (wordVector[2] != "HTTP/1.1")
		return BuildResponse("400 BAD REQUEST");
	_method = wordVector[0];
	_uri = wordVector[1];
	_protocol = wordVector[2];
	return (0);
}

int	RequestHandler::OpenFile(void)
{
	_fd_response = access(_uri.c_str(), F_OK); //checks if file exists
	if (_fd_response == -1)
		return BuildResponse("404 NOT FOUND");
	if ("GET" == _method)
	{
		_fd_response = access(_uri.c_str(), R_OK); //checks if file is redable
		if (_fd_response == -1)
			return BuildResponse("403 PERMISSION DENIED");
		std::ifstream infile(_uri, std::ios::in);
		if (infile.fail())
			return BuildResponse("500 INTERNAL SERVER ERROR");
		infile.seekg(0, std::ios::end);
		_fd_length = infile.tellg();
		infile.seekg(0, std::ios::beg);
		infile.close();
		_fd_response = open(_uri.c_str(), O_RDONLY);
		if (_fd_response == -1)
			return BuildResponse("500 INTERNAL SERVER ERROR");
		return BuildResponse("200 OK");
	}
	if ("POST" == _method)
	{
		_fd_response = access(_uri.c_str(), W_OK); //checks if file is writeble
		if (_fd_response == -1)
			return BuildResponse("403 PERMISSION DENIED");
		/*
		infile.seekg(0, std::ios::end);
		std::size_t length = infile.tellg();
		infile.seekg(0, std::ios::beg);
		infile.close();
		*/
		//write a mini backend test
	}
	if ("DELETE" == _method)
	{
		//adinoal headers
		//fd body
	}
	return (0);
}

void	RequestHandler::setRequestMsg(std::string msg) 
{
	_msg = msg;
}

int	RequestHandler::ParseRequestMsg(void)
{
	std::istringstream	iss;
	std::string			line;

	iss.str(_msg);
	std::getline(iss, line);//firs line is special in HTTP it contains the method uri and protocol
	if (ParseRequestLine(line))
		return (1);
	while (1)//this loop just gets all the header values
	{
		std::getline(iss, line);
		_request_header += line;
		if (iss.eof())
			break;
		if (iss.fail())
			return BuildResponse("500 INTERNAL SERVER ERROR");
	}
	if (OpenFile())
		return (1);
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

std::string	RequestHandler::getResponse(void) const
{
	return (_response_header + _response_body);
}

// Member functions

Location	*RequestHandler::getLocation(std::string url) const
{
	for (std::size_t it = 0; it < _srv_map.size(); it++)
	{
		if (_srv_map[it].getLocation(_port, url))
			return (_srv_map[it].getLocation(_port, url));
	}
	return NULL;
}

//TODO remove all the stuff and move it to different function (testFunction or something)
void	RequestHandler::addToRequestMsg(const std::string &msg);

void	RequestHandler::setUrlStruct(std::string full_url)
{
	std::size_t	q_pos = full_url.find('?');
	if (q_pos == std::string::npos)
		q_pos = full_url.length();
	_url.path = full_url.substr(0, q_pos);
	_url.querry = full_url.substr(q_pos, full_url.length());//
}

	_complete_request += msg;
	crlf_pos = _complete_request.find("\r\n\r\n");
	//return BuildResponse("Bad response\r\n");does the server w to recive \r\n\r\n or does it see it every time even if the reqeust is really big
	std::cout << "Important: " << crlf_pos << std::endl;
	size_req = _complete_request.size();
	if (crlf_pos != std::string::npos)
	{
		setRequestMsg(_complete_request);
		ParseRequestMsg();
	}
void	RequestHandler::setClientIp(std::string ip)
{
	_client_ip = ip;
}

void	RequestHandler::setCgiError()
{
	_cgi_error = true;
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
	// _method_header = _complete_request.substr(0, _complete_request.find_first_of("\r\n"));

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

// 2 functions below just for a bit of testing stuff

//TODO after parsing the request need to check for which server it is (first using port, then if multiple servers on same port the servername/host and then location)
void	RequestHandler::addToRequestMsg(char *msg, int bytes_received)
{
	size_t	crlf_pos;

	_complete_request.append(msg, bytes_received);
	if (!isprint(_complete_request[0])) // this is for https and bad requests
	{
		_is_request_complete = true;
		return (setServerError(&_response_body, &_response_header, "400 Bad Request"));
	}
	crlf_pos = _complete_request.find("\r\n\r\n");
	if (crlf_pos != std::string::npos)
	{
		if (_is_request_header_done == false)
		{
			makeHeaderMap();
			_is_request_header_done = true;
		}
		_request_body = _complete_request.substr(crlf_pos, std::string::npos);
		if (_headermap["Content-Length"].length() == 0)
			_is_request_complete = true;
		else if (_request_body.size() >= std::stoul(_headermap["Content-Length"]))
			_is_request_complete = true;
		if (_is_request_complete)
			testFunction();
	}
}
