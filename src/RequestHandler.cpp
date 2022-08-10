/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 11:01:06 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/10 14:47:19 by alkrusts      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include <fcntl.h>
#include <fstream>
#include <filesystem>
#include <filesystem>

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

void	RequestHandler::buildResponse(std::string path)
{
	if (path == "BAD REQUEST")
	{
		int fd = open("var/www/500.html", O_RDONLY);
		//if (fd == -1)
		//	return //internal server error or 403 forbiden etc or 404
		_response.fd = fd;
		_response.header = "HTTP/1.1 Bad Request 400\r\nServer: ft_webserver\r\nContent-Type: text/html\r\n";
		_header.push_back({"Server", "ft_webserver"});
	}
}

void	RequestHandler::ParseRequestLine(std::string line)
{
	const std::vector<std::string>	wordVector = cpp_split(line);

	if (wordVector.size() != 3)
		return buildResponse("BAD REQUEST");
	if (wordVector[0] != "GET" && wordVector[0] != "DELETE" && wordVector[0] != "POST")
		return buildResponse("BAD REQUEST");
	if (wordVector[1][0] != '/')
		return buildResponse("BAD REQUEST");
	if (wordVector[2] != "HTTP/1.1")
		return buildResponse("BAD REQUEST");
}

void	RequestHandler::setRequestMsg(std::string msg) 
{
	std::string	line;

	_msg = msg;

	std::vector<std::vector<std::string>>	request_line;
	//std::cout << "RAW DATA: " << msg << " :END OF DATA" << std::endl;

	std::istringstream iss(msg);

	std::getline(iss, line);

	//std::cout << "Hi I am getline and this is what I return: " << std::ios_base::iostate() << std::endl;

	ParseRequestLine(line);
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
