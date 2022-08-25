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
	if (_status_line.length() >= 3)
	{
		if (stoi(_status_line.substr(0, 3)) >= 300 && stoi(_status_line.substr(0, 3)) <= 310)
		{
			if (_is_folder)
				_response_header += "Location: " + _uri + "/\r\n";
			else
				_response_header += "Location: " + _uri + "\r\n";
		}	
		else if (stoi(_status_line.substr(0, 3)) < 300 && !_matching_location.getReturnCode().empty())
		{
				_response_header += "Location: " + _matching_location.getReturnCode() + "\r\n";
				_status_line = _matching_location.getReturnCode() + "Moved";
		}
	}
	_response_header = "HTTP/1.1 " + _status_line + "\r\n" + _response_header;
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

	std::cout << "HALLO" << std::endl;
	if (server_root.at(0) != '/')
		server_root = "/" + server_root;
	_file_to_get = _server_start_dir + server_root + _uri;
	std::cout << "uri: " << uri_dir << std::endl;
	_requested_dir = trim(uri_dir, "/");
	std::cout << "requested dir : " << _requested_dir << std::endl;
	requested_loc = getcwd(buf, 4096);//TO DO CHECK FOR ERROR HERE!
	if (requested_loc.empty())
		setResponseStatus("500 INTERNAL SERVER ERROR");
	else
	{
		best_match = 0;
		std::cout << "bagn: " << loc_iter->first << std::endl;
		while (loc_iter != loc_iter_end)
		{
			server_loc = trim(loc_iter->first, "/");
			std::cout << "Hallo: " << loc_iter->first << std::endl;
			std::cout << "server loc : "<< server_loc << "requested dir: " << _requested_dir << std::endl;
			if (best_match < LengthOfMatch(server_loc, _requested_dir) || (server_loc == "" && _requested_dir == ""))
			{
				std::cout << "BEST MATCH: " << loc_iter->first << std::endl;
				best_match = LengthOfMatch(server_loc, _requested_dir);
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
	//loop over defalut response pages
	if (_fd <= 0 && _response_body.empty())
	{
		setContentType("text/html");
		_response_body = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n";
		_response_body += "<title>" + _status_line + "</title>\n</head>\n<body bgcolor=\"white\">";
		_response_body += "<center><h1>" + _status_line + "</h1></center>\n</body>\n</html>";
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
	std::cout << "Open this: " << file_to_open << std::endl;
	if (access(file_to_open.c_str(), F_OK) == -1)
		setResponseStatus("404 NOT FOUND");
	else
	{
		if (getRequestMethod() == "GET")
		{
			if (access(file_to_open.c_str(), R_OK) == -1 || !getMatchingLocation().getMethodGet())
				setResponseStatus("403 FORBIDEN");
			else
			{
				if (_uri.back() == '/')
				{
					if (!_matching_location.getIndex().empty())
					{
						file_to_open += _matching_location.getIndex();
						std::ifstream infile(file_to_open.c_str(), std::ios::in);
						_content_type = getContentType(_matching_location.getIndex());
						std::cout << "content-type: " << content_type << std::endl;
						infile.seekg(0, std::ios::end);
						std::size_t length  = infile.tellg();
						infile.seekg(0, std::ios::beg);
						_file_size = length;
						_fd = open(file_to_open.c_str(), O_RDONLY);
						setResponseStatus("200 OK");
					}
					else if (_matching_location.getAutoIndex())
					{
						_response_body = AutoIndexGenerator("var/www/html", "/" + _requested_dir).getDirectoryIndex();
						setResponseStatus("200 OK");
					}
					else
						setResponseStatus("404 OK");
					return ;
				}
				else
				{
					//std::cout << "REquested: " << getRequestMethod() << std::endl;
					_file_name = file_to_open.substr(file_to_open.find_last_of("/"), file_to_open.length());
					//std::cout << "FILE NAME: " << _file_name << std::endl;
					if (checkPath(file_to_open) == IS_FILE)
					{
						std::ifstream infile(file_to_open.c_str(), std::ios::in);
						_content_type = getContentType(_file_name);
						std::cout << "content-type: " << content_type << std::endl;
						infile.seekg(0, std::ios::end);
						std::size_t length  = infile.tellg();
						infile.seekg(0, std::ios::beg);
						_file_size = length;
						_fd = open(file_to_open.c_str(), O_RDONLY);
						//std::cout << "TEST: "<< _fd << std::endl;
						setResponseStatus("200 OK");
					}
					else if (checkPath(file_to_open) == IS_DIR)
					{
						setResponseStatus("301 Moved Permanently");
						_is_folder = true;
					}
					else
					{
						std::cout << "Ither stat failed or its a sym link etc. " << std::endl;
						setResponseStatus("500 INTERNAL SERVER ERROR");
					}
				}
			}
		}
		else if (getRequestMethod() == "POST")
		{
			if (!getMatchingLocation().getMethodPost())
				setResponseStatus("403 FORBIDEN");
			else
			{
				CgiHandler cgi(*this);
				_response_body = cgi.execute();
			}
		}
		else if (getRequestMethod() == "DELETE")
		{
			if (!getMatchingLocation().getMethodDel())
				setResponseStatus("403 FORBIDEN");
			if (remove(file_to_open.c_str()) == -1)
				setResponseStatus("500 INTERNAL SERVER ERROR");
			else
				setResponseStatus("204 No Content");
		}
		else
			setResponseStatus("405 Method Not Allowed");
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

void	RequestHandler::ParseResponse(void)
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
}

void	RequestHandler::BuildResponsePage(void)
{
	t_strmap	error_pages;
	t_strmap::const_iterator iter_begin;
	t_strmap::const_iterator iter_end;
	bool		hit;

	hit = false;
	error_pages = _server.getErrorPage();
	iter_begin = error_pages.begin();
	iter_end = error_pages.end();
	while (iter_begin != iter_end)
	{
		//std::cout << "f: " << iter_begin->first << "s: " << iter_begin->second << std::endl;
		if (_status_line.substr(0, 3) == iter_begin->first)
		{
			_fd = open(("/" + _server_start_dir + "/" + _server.getServerRoot() + "/" + iter_begin->second).c_str(), O_RDONLY);
			if (_fd > 0)
			{
				std::ifstream infile("/" + _server_start_dir + "/" + _server.getServerRoot() + "/" + iter_begin->second, std::ios::in);
				_content_type = getContentType(iter_begin->second);
				std::cout << "content-type: " << _content_type << std::endl;
				infile.seekg(0, std::ios::end);
				std::size_t length  = infile.tellg();
				infile.seekg(0, std::ios::beg);
				_file_size = length;
				hit = true;
			}
		}
		iter_begin++;
	}
	if (!hit)
		BuildDefaultResponsePage();
}

void	RequestHandler::test(void)
{
	size_t	crlf_pos;

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
				ParseResponse();
				BuildResponsePage();
				BuildResponseHeader();
				//return 301 adsfadsf ladhs
			}
		}
	}
}

void	RequestHandler::addToRequestMsg(char *msg, int bytes_received)
{
	_complete_request.append(msg, bytes_received);
	if (!isprint(_complete_request[0])) // this is for https and bad requests
	{
		_is_request_complete = true;
		setResponseStatus("400 BAD REQUEST");
		BuildResponsePage();
		BuildResponseHeader();
		return ;
	}
	else
		test();
}
