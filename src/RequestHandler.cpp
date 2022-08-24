/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 11:01:06 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/24 14:18:39 by alkrusts      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "CgiHandler.hpp"
#include "Utils.hpp"

#include <fcntl.h>
#include <fstream>
#include <unistd.h>

//A server SHOULD return 414 (Request-URI Too Long) status if a URI is longer than the server can handle (see section 10.4.15).

// Constructors
RequestHandler::RequestHandler(const t_servmap &srv_map) : 
		_srv_map(srv_map), _is_request_complete(false), 
		_send_file(false), _cgi_error(false), _fd(0), _file_size(0)
{
	char	buf[4096];

	_server_start_dir = getcwd(buf, 4096);
	//here we need to quite the server
	//or internal server errro
	_host = "";
	_status_line = "";
	_content_type = "";
}

RequestHandler::RequestHandler(const RequestHandler &copy)
{
	char	buf[4096];

	_server_start_dir = getcwd(buf, 4096);
	//here we need to quite the server
	(void) copy;
}

// Destructor
RequestHandler::~RequestHandler()
{
	_host = "";
	_status_line = "";
	_content_type = "";
}

// Operators
RequestHandler & RequestHandler::operator=(const RequestHandler &assign)
{
	(void) assign;
	return *this;
}

//Getters

const std::string		&RequestHandler::getRequestBody(void) const
{
	return _request_body;
}

const std::string		&RequestHandler::getRequestMethod(void) const
{
	return _request_method;
}

const std::string	&RequestHandler::getHost(void) const
{
	return _host;
}

std::string		RequestHandler::getRequestProtocol(void) const
{
	return _request_protocol;
}

int	RequestHandler::getFileDescriptor() const
{
	return this->_fd;
}

std::size_t	RequestHandler::getFileSize() const
{
	return this->_file_size;
}

std::string	RequestHandler::getResponse() const
{
	return this->_response_header + this->_response_body;
}

t_strmap	RequestHandler::getHeaderMap() const
{
	return this->_headermap;
}

std::string	RequestHandler::getUri(void) const
{
	return _uri;
}

t_url	RequestHandler::getUrl() const
{
	return this->_url;
}

int	RequestHandler::getPort() const
{
	return this->_port;
}

std::string RequestHandler::getCompleteRequest() const
{
	return this->_complete_request;
}

std::string	RequestHandler::getClientIp() const
{
	return this->_client_ip;
}

const Location	&RequestHandler::getMatchingLocation(void) const
{
	return _matching_location;
}

// Setters

void	RequestHandler::setMatchingDir(const std::string &matching_dir)
{
	_matching_dir = matching_dir;
}

void	RequestHandler::setCompeleteRequest(const std::string &request_msg)
{
	_complete_request = request_msg;
}

void	RequestHandler::setMatchingLocation(const Location &location)
{
	_matching_location = location;
}

void	RequestHandler::setServer(const Server &server)
{
	_server = server;
}

void	RequestHandler::setContentType(const std::string &type)
{
	_content_type = type;
}

void	RequestHandler::setFileName(const std::string &name)
{
	_file_name = name;
}

void	RequestHandler::setFdBody(int fd)
{
	_fd = fd;
}

void	RequestHandler::setHost(const std::string &name)
{
	_host = name;
}

void	RequestHandler::setResponseStatus(const std::string &status_line)
{
	_status_line = status_line;
}

void	RequestHandler::setUrlStruct(const std::string &full_url)
{
	std::size_t	q_pos = full_url.find('?');
	if (q_pos == std::string::npos)
		q_pos = full_url.length();
	_url.path = full_url.substr(0, q_pos);
	_url.querry = full_url.substr(q_pos, full_url.length());
}

void	RequestHandler::setSocket(int socket)
{
	_client_socket = socket;
}

void	RequestHandler::setPort(int port)
{
	_port = port;
}

void	RequestHandler::setClientIp(const std::string &ip)
{
	this->_client_ip = ip;
}

void	RequestHandler::setCgiError(void)
{
	_cgi_error = true;
}

void	RequestHandler::setResponseCompelete(const bool &status)
{
	_is_request_complete = status;
}

const char	*RequestHandler::getHeader(void) const
{
	return (_response_header.c_str());
}

void	RequestHandler::BuildResponseHeader(void)
{
	std::size_t	len = _file_size;

	if (_fd <= 0)
		len = _response_body.length();
	_response_header += "HTTP/1.1 " + _status_line + "\r\n";
	_response_header += "Server: " + _host +  "\r\n";
	_response_header += "Content-Length: " + std::to_string(len) + "\r\n";
	_response_header += "Content-Type: " + _content_type + "\r\n\r\n";
}

/*
 *culr localhost:4242/test
 * root file test
 *
 * test
 *	html
 * ROOT:
 * check if auto index
 * AutoIndexGenerator(root + uri);
 */

void	RequestHandler::FindTheRightLocationForUri(void)
{
	char						buf[4096];
	t_locmap::const_iterator	loc_iter = _server.getLocationMap().begin();
	t_locmap::const_iterator	loc_iter_end = _server.getLocationMap().end();
	std::string					uri_dir = _uri.substr(0, _uri.find_last_of("/"));
	std::string					server_root = _server.getServerRoot();
	std::string					requested_loc;
	std::string					server_loc;
	std::size_t					best_match;
	std::string					requested_dir;

	if (server_root.at(0) != '/')
		server_root = "/" + server_root;
	_file_to_get = _server_start_dir + server_root + _uri;
	requested_dir = _server_start_dir + server_root + uri_dir;
	std::cout << "this is where to :" << requested_dir << std::endl;
	//if (chdir(requested_dir.c_str()) == -1)
//////		setResponseStatus("404 NOT FOUND");
	requested_dir = trim(uri_dir, "/");
	requested_loc = getcwd(buf, 4096);//TO DO CHECK FOR ERROR HERE!
	if (requested_loc.empty())
		setResponseStatus("500 INTERNAL SERVER ERROR");
	else
	{
		//std::cout << " This is loc: " + requested_loc << " this is requested dir " + requested_dir << std::endl;
		best_match = 0;
		while (loc_iter != loc_iter_end)
		{
			server_loc = trim(loc_iter->first, "/");
			if (best_match < LengthOfMatch(server_loc, requested_dir) || (server_loc == "" && requested_dir == ""))
			{
				best_match = LengthOfMatch(server_loc, requested_dir);
				setMatchingLocation(*(loc_iter->second));
				setMatchingDir(loc_iter->first);
			}
			loc_iter++;
		}
		if (chdir(_server_start_dir.c_str()) == -1)
			setResponseStatus("500 INTERNAL SERVER ERROR");
	}
}

void	RequestHandler::BuildDefaultResponsePage(void)
{
	setContentType("text/html");
	_response_body = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n";
	_response_body += "<title>" + _status_line + "</title>\n</head>\n<body bgcolor=\"white\">";
	_response_body += "<center><h1>" + _status_line + "</h1></center>\n</body>\n</html>";
}

void	RequestHandler::FindServer(void)
{
	t_servmap::const_iterator serv_iterator = _srv_map.begin();

	while (serv_iterator != _srv_map.end())
	{
		const std::vector<int>				&ports = serv_iterator->getServerPort();
		const t_vecstr						&server_names = serv_iterator->getServerNames();
		std::vector<int>::const_iterator	port_iter = ports.begin();

		while (port_iter != ports.end())
		{
			if (*port_iter == _port)
			{
				std::vector<std::string>::const_iterator server_name_iter = server_names.begin();
				if (getHost() == "")
				{
					setHost(*server_name_iter);
					setServer(*serv_iterator);
				}

				while (server_name_iter != server_names.end())
				{
					if (_request_host == *server_name_iter)
					{
						setHost(*server_name_iter);
						setServer(*serv_iterator);
						return ;
					}
					server_name_iter++;
				}
			}
				port_iter++;
		}
		serv_iterator++;
	}
}

/*
void	RequestHandler::CheckUserDefinedStatusPage(Server server)
{
	const t_strmap &custom_error_pages = server.getErrorPage();
	t_strmap::const_iterator user_defined_page_iter = custom_error_pages.begin();

	for (user_defined_page_iter; user_defined_page_iter != custom_error_pages.end(); user_defined_page_iter++)
	{
		if (response_status.substr(0, 3) == user_defined_page_iter->first)
		{
			std::ifstream infile((user_defined_page_iter->second).c_str(), std::ios::in);

			if (infile.fail())
			{
				BuildDefaultResponseBody(response_status);
				BuildResponseHeader();
				infile.close();
				return ;
			}
			infile.seekg(0, std::ios::end);
			std::size_t length = infile.tellg();
			infile.seekg(0, std::ios::beg);
			infile.close();
			BuildResponseHeader();
		}
	}
}
*/

void	RequestHandler::ParseRequestLine(void)
{
	std::vector<std::string>	wordVector;
	std::istringstream			iss;
	std::string					line;

	iss.str(_complete_request);
	std::getline(iss, line);
	wordVector = cpp_split(line, ' ');
	if ((wordVector.size() != 3) || 
			(wordVector[0] != "GET" && wordVector[0] != "DELETE" && wordVector[0] != "POST") ||
			(wordVector[1][0] != '/') ||
			(wordVector[2] != "HTTP/1.1"))
		setResponseStatus("400 BAD REQUEST");
	else
	{
		_uri = HexToStr(wordVector[1]);
		if (_uri == "")
			setResponseStatus("400 BAD REQUEST");
		else
		{
			_request_method = wordVector[0];
			_request_protocol = wordVector[2];
		}
	}
}



void	RequestHandler::OpenFile(void)
{
	std::string	file_to_open;
	std::string content_type;

	if (!_matching_location.getRootPath().empty())
		 file_to_open = _server_start_dir + "/" + _matching_location.getRootPath() + _uri;
	else
		 file_to_open = _server_start_dir + "/" + _server.getServerRoot() + _uri;
	if (access(file_to_open.c_str(), F_OK) == -1)
	{
		setResponseStatus("404 NOT FOUND");
		BuildDefaultResponsePage();//TO DO  add user response custom
		BuildResponseHeader();
		//build body
		return ;
	}
	if (getRequestMethod() == "GET" && getMatchingLocation().getMethodGet())
	{
		if (access(file_to_open.c_str(), R_OK) == -1)
		{
			setResponseStatus("403 FORBIDEN");
			return ;
		}
		if (_uri.back() == '/' && _matching_location.getAutoIndex())
		{
			AutoIndexGenerator("/var/www/html", "test");
			setResponseStatus("200 OK");
			return ;
		}
		else
		{
			std::cout << "REquested: " << getRequestMethod() << std::endl;
			_file_name = file_to_open.substr(file_to_open.find_last_of("/"), file_to_open.length());
			std::cout << "FILE NAME: " << _file_name << std::endl;
			if (checkPath(file_to_open) == IS_FILE)
			{
				std::ifstream infile(file_to_open.c_str(), std::ios::in);
				content_type = getContentType(_file_name);
				std::cout << "content-type: " << content_type << std::endl;
				infile.seekg(0, std::ios::end);
				std::size_t length  = infile.tellg();
				infile.seekg(0, std::ios::beg);
				_file_size = length;
				_fd = open(file_to_open.c_str(), O_RDONLY);
				setResponseStatus("200 OK");
			}
			else if (checkPath(file_to_open) == IS_DIR)
			{
				//make 301
				std::ifstream infile(file_to_open.c_str(), std::ios::in);
				content_type = getContentType(_file_name);
				std::cout << "content-type: " << content_type << std::endl;
				infile.seekg(0, std::ios::end);
				std::size_t length  = infile.tellg();
				infile.seekg(0, std::ios::beg);
				_file_size = length;
				setResponseStatus("200 OK");
			}
			else
			{
				std::cout << "Ither stat failed or its a sym link etc. " << std::endl;
				setResponseStatus("500 INTERNAL SERVER ERROR");
			}
		}
	}
	if (getRequestMethod() == "POST" && getMatchingLocation().getMethodPost())
	{
		std::cout << "REquested: " << getRequestMethod() << std::endl;
		if (getMatchingLocation().getMethodPost())
		{
			CgiHandler cgi(*this);
			cgi.execute();
		}
	}
	if (getRequestMethod() == "DELETE" && getMatchingLocation().getMethodDel())
	{
		std::cout << "REquested: " << getRequestMethod() << std::endl;
		if (getMatchingLocation().getMethodDel())
		{

		}
	}
}

bool	RequestHandler::isRequestComplete() const
{
	return _is_request_complete;
}

// Member functions
void	RequestHandler::makeHeaderMap()
{
	std::vector<std::string>	split;
	std::size_t					pos;
	std::size_t					last_pos;
	std::size_t					len;
	std::size_t					first_pos;
	std::size_t					end_pos;

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
	first_pos = _complete_request.find_first_of(' ') + 1;
	end_pos = _complete_request.find_first_of(' ', first_pos + 1);
	setUrlStruct(_complete_request.substr(first_pos, end_pos - first_pos));
	_request_method = _complete_request.substr(0, first_pos - 1);
	//_method_header = _complete_request.substr(0, _complete_request.find_first_of("\r\n"));

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

void	RequestHandler::ParseHeaderMap(void)
{
	for (t_strmap::const_iterator headerMap_iter = _headermap.begin(); headerMap_iter != _headermap.end(); headerMap_iter++)
	{
		/*
		 *			HTTP/1.1 206 Partial content
				   Date: Wed, 15 Nov 1995 06:25:24 GMT
				   Last-Modified: Wed, 15 Nov 1995 04:58:08 GMT
				   Content-Range: bytes 21010-47021/47022
				   Content-Length: 26012
				   Content-Type: image/gif
		 */
		if (headerMap_iter->first == "Host")
		{
			std::size_t	pos = headerMap_iter->second.find(':');
			_request_host = headerMap_iter->second.substr(0, pos);
		}
	}
}

bool	RequestHandler::isResponseDone(void) const
{
	if (_status_line == "")
		return (false);
	return (true);

}

void	RequestHandler::MakeResponse(void)
{
	std::size_t	index;
	void	(RequestHandler::*fptr[])(void) = {
		&RequestHandler::ParseHeaderMap,
		&RequestHandler::ParseRequestLine,
		&RequestHandler::FindServer,
		&RequestHandler::FindTheRightLocationForUri,
		&RequestHandler::OpenFile,
		NULL
	}; 

	index = 0;
	while (fptr[index] != NULL && !isResponseDone())
	{
		(this->*fptr[index])();
		index++;
	}
	BuildResponseHeader();
	setResponseCompelete(true);
}

void	RequestHandler::addToRequestMsg(char *msg, int bytes_received)
{
	size_t	crlf_pos;

	_complete_request.append(msg, bytes_received);
	if (!isprint(_complete_request[0])) // this is for https and bad requests
	{
		_is_request_complete = true;
		//return (setServerError(&_response_body, &_response_header, "400 Bad Request"));

		return ;
	}
	crlf_pos = _complete_request.find("\r\n\r\n");
	if (crlf_pos != std::string::npos)
	{
		if (_is_request_header_done == false)
		{
			_request_header = _complete_request.substr(0, crlf_pos);
			makeHeaderMap();
			_is_request_header_done = true;
		}
		if (_is_request_header_done)
		{
			_request_body = _complete_request.substr(crlf_pos + 4, std::string::npos);
			if (_headermap.find("Content-Length")  == _headermap.end())
				_is_request_complete = true;
			else if (_request_body.length() >= std::stoul(_headermap["Content-Length"]))
				_is_request_complete = true;
			if (_is_request_complete)
			{
				MakeResponse();
				BuildDefaultResponsePage();//TO DO  add user response custom
				BuildResponseHeader();
			}
		}
	}
}
