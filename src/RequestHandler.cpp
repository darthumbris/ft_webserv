/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 11:01:06 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/21 15:09:02 by alkrusts      ########   odam.nl         */
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
		_has_remaining_request(false), _send_file(false), _fd_response(0),
		_file_size(0)
{
	_host = "";
	_status_line = "";
	_content_type = "";
}

RequestHandler::RequestHandler(const RequestHandler &copy)
{
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


const std::string	&RequestHandler::getMatchingDir(void) const
{
	return _matching_dir;
}

const std::string	&RequestHandler::getStatusLine(void) const
{
	return _status_line;
}

const Server &RequestHandler::getServer(void) const
{
	return _server;
}

const std::string	&RequestHandler::getHost(void) const
{
	return _host;
}

std::string		RequestHandler::getRequestMethod(void) const
{
	return _request_method;
}

std::string		RequestHandler::getRequestProtocol(void) const
{
	return _request_protocol;
}

std::string	RequestHandler::getRemainingRequestMsg() const
{
	return this->_remaining_request;
}

int	RequestHandler::getFileDescriptor() const
{
	return this->_fd_response;
}

std::size_t	RequestHandler::getFileSize() const
{
	return this->_file_size;
}

int	RequestHandler::getResponseBody(void) const
{
	return this->_fd_response;
}

std::string	RequestHandler::getResponse(void) const
{
	return ("");
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
	return _port;
}

std::string RequestHandler::getCompleteRequest() const
{
	return this->_complete_request;
}

const Location	&RequestHandler::getMatchingLocation(void) const
{
	return _matching_location;
}

void	RequestHandler::setMatchingDir(const std::string &matching_dir)
{
	_matching_dir = matching_dir;
}

// Setters

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
	_fd_response = fd;
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

void	RequestHandler::setRequestMsg(const std::string &msg) 
{
	_msg = msg;
}

void	RequestHandler::setSocket(int socket)
{
	_client_socket = socket;
}

void	RequestHandler::setPort(int port)
{
	_port = port;
}

void	RequestHandler::setCgiError(void)
{
	_cgi_error = true;
}

void	RequestHandler::setResponseCompelete(const bool &status)
{
	_is_request_complete = status;
}

std::string	RequestHandler::getClientIp() const
{
	return _client_ip;
}

const char	*RequestHandler::getHeader(void) const
{
	return (_response_header.c_str());
}

int			RequestHandler::getBody(void) const
{
	return (_fd_response);
}

void	RequestHandler::BuildResponseHeader(void)
{
	_response_header += "HTTP/1.1 " + _status_line + "\r\n";
	_response_header += "Server: " + _host +  "\r\n";
	_response_header += "Content-Length: " + std::to_string(_fd_length) + "\r\n";
	_response_header += "Content-Type: " + _content_type + "\r\n\r\n";
}

/*
 * root + location
 * first we need to know if this path exists at all
 * uri test/index.html
 * /var/www/html/test
 */

/*
void	RequestHandler::FindBestFithLocation(void)
{
	std::map<Location, std::string>::const_iterator	path_iter = _location_paths.begin();
	
	wordVector = cpp_split(line, );
	while (path_iter != _location_paths.end())
	{
		path_iter++;
	}
}

void	RequestHandler::FindLocationPath(void)
{
	t_locmap::const_iterator	loc_iter = _server.getLocationMap().begin();

	while (loc_iter != _locations.end())
	{
		_location_paths.push_back(*loc_iter, loc_iter->first + loc_iter->second->getRootPath());
		loc_iter++;
	}
}
*/

void	RequestHandler::FindTheRightLocationForUri(void)
{
	t_locmap::const_iterator	loc_iter = _server.getLocationMap().begin();
	t_locmap::const_iterator	loc_iter_end = _server.getLocationMap().end();
	const std::string			&server_root = _server.getServerRoot();
	std::string					abs_uri_dir;
	std::string					abs_location_dir;
	std::size_t					best_match;
	std::string					uri_dir;
	std::string					new_dir;
	std::string					old_dir;
	char						buf[4096];

	if (_uri.back() == '/')
	{
		uri_dir = _uri;
		uri_dir.pop_back();
	}
	else
		uri_dir = _uri;
	abs_uri_dir = server_root + uri_dir.substr(0, uri_dir.find_last_of("/"));
	if (abs_uri_dir.back() != '/')
		abs_uri_dir += "/";
	if (abs_uri_dir.at(0) != '/')
		abs_uri_dir.append("/");
	if (abs_uri_dir == "")
		abs_uri_dir = "/";
	old_dir = getcwd(buf, 4096);
	if (chdir(abs_uri_dir.c_str()) == -1)
		setResponseStatus("404 NOT FOUND");
	new_dir = getcwd(buf, 4096);
	std::cout << new_dir << std::endl;
	if (chdir(old_dir.c_str()) == -1)
		setResponseStatus("500 INTERNAL SERVER ERROR");
	new_dir = getcwd(buf, 4096);
	std::cout << new_dir << std::endl;
	best_match = 0;
	while (loc_iter != loc_iter_end)
	{
		abs_location_dir = server_root + loc_iter->first;
		if (abs_location_dir == "")
			abs_location_dir = "/";
		if (abs_location_dir.back() != '/')
			abs_location_dir += "/";
		if (abs_location_dir.at(0) != '/')
			abs_location_dir.append("/");
		if (best_match < LengthOfMatch(abs_location_dir, abs_uri_dir))
		{
			best_match = LengthOfMatch(abs_location_dir, abs_uri_dir);
			setMatchingLocation(*(loc_iter->second));
			setMatchingDir(loc_iter->first);
		}
		loc_iter++;
	}
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
	std::istringstream				iss;
	std::string						line;

	iss.str(_msg);
	std::getline(iss, line);
	wordVector = cpp_split(line, ' ');
	if ((wordVector.size() != 3) || 
			(wordVector[0] != "GET" && wordVector[0] != "DELETE" && wordVector[0] != "POST") ||
			(wordVector[1][0] != '/') ||
			(wordVector[2] != "HTTP/1.1"))
	{
		_request_method = "FAIL";
		_uri = "FAIL";
		_request_protocol = "FAIL";
		setResponseStatus("400 BAD REQUEST");
		return ;
	}
	_request_method = wordVector[0];
	_uri = stripExesSlashes(HexToStr(wordVector[1]));
	if (_uri == "")
	{
		_request_method = "FAIL";
		_uri = "FAIL";
		_request_protocol = "FAIL";
		setResponseStatus("400 BAD REQUEST");
	}
	_request_protocol = wordVector[2];
}

void	RequestHandler::OpenFile(void)
{
	/*
	int											fd;
	std::vector<std::string>::const_iterator	path_iter = _accessible_paths.begin();
	t_methods::const_iterator					method_iter = _allowed_methods.begin();

	while (path_iter != path_iter.end())
	{
		fd = access(_uri.c_str(), F_OK); //checks if file exists
		if (fd == -1)
		{
			setFdBody(fd);
			setResponseStatus("404 NOT FOUND");
		}
		if (fd != -1)
		{
			while (method_iter != _allowed_methods.end())
			{
				if (*method_iter == _request_method && _request_method == "GET")
				{
					fd = access(_uri.c_str(), R_OK); //checks if file is redable
					if (fd == -1)
					{
						setFdBody(fd);
						setResponseStatus("403 FORBIDDEN");
						return ;
					}
					if (checkPath(_uri) == IS_DIR)
					{
						if (_uri.end() != '/')	
							_uri += "/";
						_uri = getRootPath() + _uri;
					}
					std::ifstream infile(_uri, std::ios::in);
					if (infile.fail())
					{
						setFdBody(fd);
						setResponseStatus("500 INTERNAL SERVER ERROR");
						return ;
					}
					infile.seekg(0, std::ios::end);
					_fd_length = infile.tellg();
					infile.seekg(0, std::ios::beg);
					infile.close();
					fd = open(_uri.c_str(), O_RDONLY);
					if (fd == -1)
					{
						setFdBody(fd);
						setResponseStatus("500 INTERNAL SERVER ERROR");
						return ;
					}
					setFileName(_uri.c_str());
					setContentType(getContentType(_uri));
					setFdBody(fd);
					setResponseStatus("200 OK");
				}
				if (*method_iter == _request_method && _request_method == "POST")
				{
					fd = access(_uri.c_str(), W_OK); //checks if file is writeble
					if (fd == -1)
					{
						setResponseStatus("403 FORBIDDEN");
						return ;
					}
				}
				if (*method_iter == _request_method && _request_method == "DELETE")
				{

				}
				method_iter++;
			}
		}
		path_iter++;
	}
	*/
}

bool	RequestHandler::isRequestComplete() const
{
	return _is_request_complete;
}

bool	RequestHandler::hasRemainingRequestMsg() const
{
	return _has_remaining_request;
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

void	RequestHandler::BuildDefaultResponseBody(void)
{
	_request_header += "<!DOCTYPE html> \
	<html lang='en'> \
	  <head> \
		<meta charset='UTF-8'> \
		<title>";
	_request_header += _status_line;
	_request_header += "</title> \
	  </head> \
	  <body bgcolor='white'> \
		<center><h1>";
	_request_header += _status_line;
	_request_header += "</h1></center> \
	  </body> \
	</html>\r\n\r\n";
}


int	RequestHandler::ParseHeaderMap(void)
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
	return (0);
}

//TODO after parsing the request need to check for which server it is (first using port, then if multiple servers on same port the servername/host and then location)
void	RequestHandler::addToRequestMsg(char *msg, int bytes_received)
{
	size_t	crlf_pos;

	_complete_request.append(msg, bytes_received);
	if (!isprint(_complete_request[0])) // this is for https and bad requests
	{
		setResponseStatus("400 BAD REQUEST");
		return ;
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
		{
			setRequestMsg(_complete_request);
			ParseHeaderMap();
			ParseRequestLine();
			FindServer();
			FindTheRightLocationForUri();
			OpenFile();
			BuildResponseHeader();
			setResponseCompelete(true);
		}
	}
}
