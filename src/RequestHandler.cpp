/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 11:01:06 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/11 13:27:24 by alkrusts      ########   odam.nl         */
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

std::string	RequestHandler::openFile(const char *path, std::string method)
{
	int	status;
	int	mode;

	status = access(path, mode);
	if (status
}
 
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

RequestHandler::RequestHandler(Server *server) : _server(server)
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
t_response	RequestHandler::getResponse() const
{
	return this->_response;
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
		wordVector.push_back(line.substr(prev, std::string::npos));
	return (wordVector);
}

char	*RequestHandler::getHeader(void) const
{
	return (_header);
}

int		RequestHandler::getBody(void) const
{
	return (_body);
}

void	RequestHandler::buildResponse(std::string request)
{
	std::string	TmpHeader;

	if (request == "INTERNAL SERVER ERROR 500")
	{
		TmpHeader += "HTTP/1.1 Internal Server Error 500\r\n";
		TmpHeader += "Server: ft_webserver\r\n";
		TmpHeader += "Content-Type: text/html\r\n";
		TmpHeader += INTERNAL_SERVER_ERROR_500;
	}
	/*
	 * CLIENT ERROR 4xx
	 */
	if (request == "BAD REQUEST 400")
	{
		_body = open("var/www/400.html", O_RDONLY);
		if (_body == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		TmpHeader += "HTTP/1.1 Bad Request 400\r\n";
		TmpHeader += "Server: ft_webserver\r\n";
		TmpHeader += "Content-Type: text/html\r\n";
	}
	if (request == "UNAUTHORIZED 401")
	{
		_body = open("var/www/401.html", O_RDONLY);
		if (_body == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		TmpHeader += "HTTP/1.1 Bad Request 401\r\n";
		TmpHeader += "Server: ft_webserver\r\n";
		TmpHeader += "Content-Type: text/html\r\n";
	}
	if (request == "PAYMENT REQUIRED 402") //this code is reserver for future use XD rfc2626
	{
		_body = open("var/www/402.html", O_RDONLY);
		if (_body == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		TmpHeader += "HTTP/1.1 Payment Required 402\r\n";
		TmpHeader += "Server: ft_webserver\r\n";
		TmpHeader += "Content-Type: text/html\r\n";
	}
	if (request == "NOT FOUND 403")
	{
		_body = open("var/www/403.html", O_RDONLY);
		if (_body == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		TmpHeader += "HTTP/1.1 Not Found 404\r\n";
		TmpHeader += "Server: ft_webserver\r\n";
		TmpHeader += "Content-Type: text/html\r\n";
	}	
	if (request == "NOT FOUND 404")
	{
		_body = open("var/www/404.html", O_RDONLY);
		if (_body == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		TmpHeader += "HTTP/1.1 Not Found 404\r\n";
		TmpHeader += "Server: ft_webserver\r\n"; TmpHeader += "Content-Type: text/html\r\n"; }
	}
	if (request == "BAD REQUEST 400")
	{
		_body = open("var/www/400.html", O_RDONLY);
		if (_body == -1)
			return buildResponse("INTERNAL SERVER ERROR 500");
		TmpHeader += "HTTP/1.1 Bad Request 400\r\n";
		TmpHeader += "Server: ft_webserver\r\n";
		TmpHeader += "Content-Type: text/html\r\n";
	}
	_header = TmpHeader.c_str();
}

void	RequestHandler::ParseRequestLine(std::string line)
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
}

void	OpenFile(void)
{
	int	status;

	status = access(_uri, F_OK); //checks if file exists
	if (status == -1)
		return buildResponse("404 NOT FOUND");
	if ("GET" == _method)
	{
		status = access(_uri, R_OK); //checks if file is redable
		if (status == -1)
			return buildResponse("403 PERMISSION DENIED");
	}
	if ("POST" == _method)
	{
		status = access(_uri, W_OK); //checks if file is writeble
		if (status == -1)
			return buildResponse("403 PERMISSION DENIED");
	}
	if ("DELETE" == _method)
	{
	}
}

void	RequestHandler::setRequestMsg(std::string msg) 
{
	std::string	line;

	_msg = msg;
	std::istringstream iss(msg);

	std::getline(iss, line);

	ParseRequestLine(line);
	OpenFile();
	while (1)
	{
		std::getline(iss, line);
		//if (iss.fail())
		//	return (internal_server_error());
		if (iss.eof())
			break;
	}

	if (msg.find("\r\n\r\n"))
		std::cout << "end of request header" << std::endl;
	std::cout << "FD:" << _response.fd << " HAEDER: " << _response.header << " : " << _response.header << std::endl;
	//return (_response);
}
