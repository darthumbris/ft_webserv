/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: alkrusts <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 11:01:06 by alkrusts      #+#    #+#                 */
/*   Updated: 2022/08/23 15:13:46 by alkrusts      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "CgiHandler.hpp"
#include "Utils.hpp"

<<<<<<< HEAD
#include <fcntl.h>
#include <fstream>
#include <unistd.h>

//A server SHOULD return 414 (Request-URI Too Long) status if a URI is longer than the server can handle (see section 10.4.15).

// Constructors
RequestHandler::RequestHandler(const t_servmap &srv_map) : 
		_srv_map(srv_map), _is_request_complete(false), 
		_send_file(false), _fd_response(0),
		_file_size(0)
=======
// Constructors
RequestHandler::RequestHandler(const t_servmap &srv_map) : 
		_srv_map(srv_map), _is_request_complete(false), 
		_has_remaining_request(false), _send_file(false),
		_cgi_error(false), _fd(0), _file_size(0)
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
{
	char	buf[4096];

	_server_start_dir = getcwd(buf, 4096);
	_host = "";
	_status_line = "";
	_content_type = "";
}

RequestHandler::RequestHandler(const RequestHandler &copy)
{
	char	buf[4096];

	_server_start_dir = getcwd(buf, 4096);
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

const std::string		&RequestHandler::getRequestMethod(void) const
{
	return _request_method;
}

const std::string	&RequestHandler::getMatchingDir(void) const
{
	return _matching_dir;
}

<<<<<<< HEAD
const std::string	&RequestHandler::getStatusLine(void) const
{
	return _status_line;
}

const Server &RequestHandler::getServer(void) const
=======
bool	RequestHandler::hasRemainingRequestMsg() const
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
{
	return _server;
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
	return this->_fd_response;
}

std::size_t	RequestHandler::getFileSize() const
{
	return this->_file_size;
}

std::string	RequestHandler::getResponse() const
{
	return this->_response_header + this->_response_body;
}

int	RequestHandler::getFd(void) const
{
	return this->_fd_response;
}

t_strmap	RequestHandler::getHeaderMap() const
{
	return this->_headermap;
}

std::string	RequestHandler::getUri(void) const
{
	return _uri;
}

>>>>>>> alkrusts
t_url	RequestHandler::getUrl() const
{
	return this->_url;
}

int	RequestHandler::getPort() const
{
<<<<<<< HEAD
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

const std::string&	RequestHandler::getRequestHeader() const
{
<<<<<<< HEAD
	return this->_complete_request;
=======
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
>>>>>>> alkrusts
=======
	return this->_request_header;
}

const std::string&	RequestHandler::getRequestBody() const
{
	return this->_request_body;
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
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

void	RequestHandler::setSocket(int socket)
{
	_client_socket = socket;
}

void	RequestHandler::setPort(int port)
{
	_port = port;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
void	RequestHandler::setClientIp(std::string ip)
{
	this->_client_ip = ip;
}

//TODO fix this so if it can't find the location it starts looking at parent directories until it finds it
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
Location	*RequestHandler::getLocation(std::string url) const
=======
void	RequestHandler::setCgiError(void)
{
<<<<<<< HEAD
	_cgi_error = true;
}

void	RequestHandler::setResponseCompelete(const bool &status)
{
	_is_request_complete = status;
}

std::string	RequestHandler::getClientIp() const
>>>>>>> alkrusts
{
	return _client_ip;
}

const char	*RequestHandler::getHeader(void) const
{
	return (_response_header.c_str());
}

void	RequestHandler::BuildResponseHeader(void)
{
	std::size_t	len = _fd_length;

	if (_fd <= 0)
		len = _reponse_body.length();
	_response_header += "HTTP/1.1 " + _status_line + "\r\n";
	_response_header += "Server: " + _host +  "\r\n";
	_response_header += "Content-Length: " + std::to_string(len) + "\r\n";
	_response_header += "Content-Type: " + _content_type + "\r\n\r\n";
}

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
	if (chdir(requested_dir.c_str()) == -1)
		setResponseStatus("404 NOT FOUND");
	else
=======
	std::cout << "trying to get the location class for the url: " << url << std::endl;
	for (std::size_t it = 0; it < _srv_map.size(); it++)
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
	{
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
}

void	RequestHandler::setUrlStruct(std::string full_url)
{
	std::size_t	q_pos = full_url.find('?');
	if (q_pos == std::string::npos)
		q_pos = full_url.length();
	_url.path = full_url.substr(0, q_pos);
	_url.querry = full_url.substr(q_pos, full_url.length());
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
	len = _request_header.length();
	last_pos = 0;
	while (last_pos < len + 1)
	{
		pos = _request_header.find_first_of("\r\n", last_pos);
		if (pos == std::string::npos)
			pos = len;
		if (pos != last_pos)
			split.push_back(std::string(_request_header.data() + last_pos, pos - last_pos));
		last_pos = pos + 1;
	}
	std::size_t	first_pos = _request_header.find_first_of(' ') + 1;
	std::size_t	end_pos = _request_header.find_first_of(' ', first_pos + 1);
	setUrlStruct(_request_header.substr(first_pos, end_pos - first_pos));
	_request_method = _request_header.substr(0, first_pos - 1);

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
static void	RequestHandler::BuildDefaultResponsePage(void)
{
	setContentType("text/html");
	std::cout << "Server Error:"<< error << std::endl;
<<<<<<< HEAD
	//if there is not error page set!
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
=======
	*body = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n";
	*body += "<title>" + error + "</title>\n</head>\n<body bgcolor=\"white\">";
	*body += "<center><h1>" + error + "</h1></center>\n</body>\n</html>";
	*header = "HTTP/1.1 " + error + "\r\nContent-Length: ";
	*header += std::to_string((*body).length());
	*header += "\r\nConnection: keep-alive\r\nContent-Type: ";
	*header += "text/html\r\n\r\n";
}

void	RequestHandler::testFunction()
{
	// std::cout << "request header: " << _request_header << std::endl;
	// std::cout << "urlpath: " << _url.path << std::endl;
	std::string	file = "";
	std::string url = _url.path.substr(0, _url.path.find_last_of('/') + 1);
	std::string root;
	std::string path;
	if (getLocation(url))
		root = getLocation(url)->getRootPath();
	else
		return (setServerError(&_response_body, &_response_header, "404 File not Found"));
	// root.append("/");
	if (checkPath(_url.path, root) == IS_FILE)
		file += _url.path.substr(_url.path.find_last_of('/') + 1, _url.path.length());
	else
		url = _url.path;
	if (url.length() > 1 && checkPath(_url.path, root) == IS_DIR && _url.path.find_last_of('/') != _url.path.length() - 1)
		url += "/";	
	std::cout << "root: " << root << std::endl;
	std::cout << "url: " << url << std::endl;
	std::cout << "file: " << file << std::endl;
	if (_complete_request.find("GET /") != std::string::npos || _complete_request.find("POST /") != std::string::npos)
	{
		if (_url.path.find(".php") != std::string::npos || _url.path.find(".py") != std::string::npos)
		{
			CgiHandler	cgi(*this);
			_response_body = cgi.execute();
			std::cout << _response_body << std::endl;
			std::size_t content_type_start_pos = _response_body.find("Content-type:") + 13;
			std::cout << "start pos: " << content_type_start_pos << std::endl;
			std::size_t	content_type_end_pos = _response_body.find_first_of("\r\n", content_type_start_pos);
			std::string content_type = _response_body.substr(content_type_start_pos, content_type_end_pos - content_type_start_pos + 2);
			std::cout << "content-type:" << content_type << std::endl;
			_response_body = _response_body.substr(content_type_end_pos + 2, _response_body.length());
			std::cout << "\nResponse body after substr: " << _response_body << std::endl;
			_response_header = "HTTP/1.1 200 Ok\r\n";
			_response_header += "Content-Length: ";
			_response_header += std::to_string(_response_body.length());
			_response_header += "\r\nConnection: keep-alive\r\n";
			_response_header += "Content-type: " + content_type + "\r\n\r\n";
			_complete_request.clear();
			_request_header.clear();
			std::cout << "response header: " << _response_header << std::endl;
			return;
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
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
<<<<<<< HEAD
=======
			return (setServerError(&_response_body, &_response_header, "500 Internal Server Errorasd"));
		if (file == "" && loc && loc->getIndex() != "")
			file = loc->getIndex();
		std::cout << "\n\n=======Path:" << path << std::endl;
		std::ifstream infile(path, std::ios::in);
		if (!infile.is_open())
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
		{
			_request_method = wordVector[0];
			_request_protocol = wordVector[2];
		}
	}
}



void	RequestHandler::OpenFile(void)
{
	if (getRequestMethod() == "GET")
	{
		std::cout << "REquested: " << getRequestMethod() << std::endl;
		if (getMatchingLocation().getMethodGet())
		{
<<<<<<< HEAD
			if (checkPath() == IS_FILE)
=======
			std::string content_type;
			content_type = getContentType(file);
			// if (_complete_request.find("/favicon.ico") != std::string::npos)
			// 	content_type = "image/x-icon";
			// else if (_complete_request.find("/cheese.png") != std::string::npos)
			// 	content_type = "image/png";
			// else
			// 	content_type = "text/html";
			std::cout << "content-type: " << content_type << std::endl;
			infile.seekg(0, std::ios::end);
			std::size_t length  = infile.tellg();
			infile.seekg(0, std::ios::beg);
			_file_size = length;
			_response_header = ("HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(length) + "\r\nConnection: keep-alive\r\nContent-Type: " + content_type + "\r\n\r\n");
			if (infile.fail())
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
			{
				_fd_response = open(_file_to_get.c_str(), O_RDONLY);
				std::cout << _fd_response << "  " + _file_to_get << std::endl;
				std::ifstream infile(_file_to_get.c_str(), std::ios::in);
				infile.seekg(0, std::ios::end);
				std::size_t length  = infile.tellg();
				infile.seekg(0, std::ios::beg);
				_file_size = length;
				setResponseStatus("200 OK");
			}
			if (checkPath() == IS_DIR)
			{
				//check if index is set for location
				//check if autoindex is set
				//if false return error
				//check error logic
			}
		}
	}
	if (getRequestMethod() == "POST")
	{
		std::cout << "REquested: " << getRequestMethod() << std::endl;
		if (getMatchingLocation().getMethodPost())
		{
			CgiHandler cgi(*this);
			cgi.execute();
		}
	}
	if (getRequestMethod() == "DELETE")
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
		setResponseStatus("400 BAD REQUEST");
		//MakeResponse(); here we need a repsonse !!!
	}
	else
	{
		crlf_pos = _complete_request.find("\r\n\r\n");
		if (crlf_pos != std::string::npos)
		{
<<<<<<< HEAD
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
				setResponseCompelete(false);
				MakeResponse();
			}
		}
=======
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
				testFunction();
		}
>>>>>>> adeb397b66dc98e28e81fd4be59ebd586042f888
	}
}
