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
		_fd(0), _file_size(0)
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

const std::string&	RequestHandler::getFileName() const
{
	return _file_name;
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
void	RequestHandler::setCompleteRequest(const std::string &request_msg)
{
	_complete_request = request_msg;
}

void	RequestHandler::setContentType(const std::string &type)
{
	_content_type = type;
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

void	RequestHandler::setPort(int port)
{
	_port = port;
}

void	RequestHandler::setClientIp(const std::string &ip)
{
	this->_client_ip = ip;
}

//TODO protect the stoi
void	RequestHandler::buildResponseHeader(void)
{
	std::size_t	len = _file_size;

	if (_fd <= 0)
		len = _response_body.length();
	if (_status_line.length() >= 3)
	{
		if (std::stoi(_status_line.substr(0, 3)) >= 300 && std::stoi(_status_line.substr(0, 3)) <= 310)
		{
			if (_is_folder)
				_response_header += "Location: " + _uri + "/\r\n";
			else
				_response_header += "Location: " + _uri + "\r\n";
		}	
		else if (std::stoi(_status_line.substr(0, 3)) < 300 && !_matching_location.getReturnCode().empty())
		{
				_response_header += "Location: " + _matching_location.getReturnCode() + "\r\n";
				_status_line = _matching_location.getReturnCode() + "Moved";
		}
	}
	_response_header = "HTTP/1.1 " + _status_line + "\r\n" + _response_header;
	_response_header += "Server: " + _host +  "\r\n";
	_response_header += "Content-Length: " + std::to_string(len) + "\r\n";
	_response_header += "Content-Type: " + _content_type + "\r\n\r\n";
	std::cout << "response_header: " << _response_header << std::endl;
}

void	RequestHandler::findLocationForUri(void)
{
	char		buf[4096];
	std::string	uri_dir = _uri.substr(0, _uri.find_last_of("/"));
	std::string	server_root = _server.getServerRoot();
	std::string	requested_loc;
	std::size_t	best_match = 0;

	if (server_root.at(0) != '/')
		server_root = "/" + server_root;
	_file_to_get = _server_start_dir + server_root + _uri;
	std::cout << "uri: " << uri_dir << std::endl;
	_requested_dir = trim(uri_dir, "/");
	std::cout << "requested dir : " << _requested_dir << std::endl;
	requested_loc = getcwd(buf, 4096);//TODO CHECK FOR ERROR HERE!
	if (requested_loc.empty())
		return setResponseStatus("500 INTERNAL SERVER ERROR");
	for (t_locmap::const_iterator loc_it = _server.getLocationMap().begin(); loc_it != _server.getLocationMap().end(); loc_it++)
	{
		std::string server_loc = trim(loc_it->first, "/");
		std::cout << "Hallo: " << loc_it->first << std::endl;
		std::cout << "server loc : "<< server_loc << "requested dir: " << _requested_dir << std::endl;
		if (best_match < lengthOfMatch(server_loc, _requested_dir) || (server_loc == "" && _requested_dir == ""))
		{
			std::cout << "BEST MATCH: " << loc_it->first << std::endl;
			best_match = lengthOfMatch(server_loc, _requested_dir);
			_matching_location = *(loc_it->second);
			_matching_dir = loc_it->first;
		}
	}
	if (chdir(_server_start_dir.c_str()) == -1)
		setResponseStatus("500 INTERNAL SERVER ERROR");
}

void	RequestHandler::buildDefaultResponsePage(void)
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

void	RequestHandler::findServer(void)
{
	for (t_servmap::const_iterator serv_it = _srv_map.begin(); serv_it != _srv_map.end(); serv_it++)
	{
		const t_vecstr	&serv_names = serv_it->getServerNames();

		if (serv_it->hasPort(_port))
		{
			if (_host == "")
			{
				_host = *serv_names.begin();
				_server = *serv_it;
				
			}
			if (serv_it->hasMatchingHost(_request_host))
			{
				_host = _request_host;
				_server = *serv_it;
			}
		}
	}
	if (_server.getClientBodySize() < (int)_request_body.length())
		setResponseStatus("413 Request Entity Too Large");
}


void	RequestHandler::parseRequestLine(void)
{
	t_vecstr			word_vector;
	std::istringstream	iss;
	std::string			line;

	iss.str(_complete_request);
	std::getline(iss, line);
	word_vector = cpp_split(line, ' ');
	if ((word_vector.size() != 3) || 
			(word_vector[0] != "GET" && word_vector[0] != "DELETE" && word_vector[0] != "POST") ||
			(word_vector[1][0] != '/') ||
			(word_vector[2] != "HTTP/1.1"))
		return setResponseStatus("400 BAD REQUEST");
	_uri = hexToStr(word_vector[1]);
	if (_uri == "")
		setResponseStatus("400 BAD REQUEST");
	else
	{
		_request_method = word_vector[0];
		_request_protocol = word_vector[2];
	}
}

void	RequestHandler::handleGetMethod(std::string &file_to_open)
{
	setResponseStatus("200 OK");
	if (access(file_to_open.c_str(), R_OK) == -1 || !getMatchingLocation().getMethodGet())
		return setResponseStatus("403 FORBIDEN");
	if (_uri.back() == '/')
	{
		if (!_matching_location.getIndex().empty())
		{
			file_to_open += _matching_location.getIndex();
			openFile(file_to_open);
		}
		else if (_matching_location.getAutoIndex())
			_response_body = AutoIndexGenerator(_matching_location.getRootPath(), "/" + _requested_dir).getDirectoryIndex();
		else
			setResponseStatus("404 Not Found");
	}
	else
	{
		_file_name = file_to_open.substr(file_to_open.find_last_of("/"), file_to_open.length());
		if (checkPath(file_to_open) == IS_FILE)
			openFile(file_to_open);
		else if (checkPath(file_to_open) == IS_DIR)
		{
			setResponseStatus("301 Moved Permanently");
			_is_folder = true;
		}
		else
		{
			std::cerr << "Ither stat failed or its a sym link etc. " << std::endl;
			setResponseStatus("500 INTERNAL SERVER ERROR");
		}
	}
}

void	RequestHandler::handlePostMethod()
{
	if (!getMatchingLocation().getMethodPost())
		setResponseStatus("403 FORBIDEN");
	else
	{
		CgiHandler cgi(*this);
		_response_body = cgi.execute();
	}
}

void	RequestHandler::handleDeleteMethod(std::string &file_to_open)
{
	if (!getMatchingLocation().getMethodDel())
		setResponseStatus("403 FORBIDEN");
	if (remove(file_to_open.c_str()) == -1)
		setResponseStatus("500 INTERNAL SERVER ERROR");
	else
		setResponseStatus("204 No Content");
}

void	RequestHandler::openFile(std::string& file_to_open)
{
	std::ifstream infile(file_to_open.c_str(), std::ios::in);
	_content_type = getContentType(file_to_open);
	std::cout << "content-type: " << _content_type << std::endl;
	infile.seekg(0, std::ios::end);
	std::size_t length  = infile.tellg();
	infile.seekg(0, std::ios::beg);
	_file_size = length;
	_fd = open(file_to_open.c_str(), O_RDONLY);
}

void	RequestHandler::handleRequest(void)
{
	std::string	file_to_open;

	if (!_matching_location.getRootPath().empty())
		 file_to_open = _server_start_dir + "/" + _matching_location.getRootPath() + _uri;
	else
		 file_to_open = _server_start_dir + "/" + _server.getServerRoot() + _uri;
	_file_name = file_to_open;
	std::cout << "Open this: " << file_to_open << std::endl;
	if (access(file_to_open.c_str(), F_OK) == -1)
		setResponseStatus("404 NOT FOUND");
	else
	{
		if (_request_method == "GET")
			handleGetMethod(file_to_open);
		else if (_request_method == "POST")
			handlePostMethod();
		else if (_request_method == "DELETE")
			handleDeleteMethod(file_to_open);
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
	t_vecstr		split = splitOnString(_request_header, "\r\n");
	std::size_t		first_pos;
	std::size_t		end_pos;

	first_pos = _complete_request.find_first_of(' ') + 1;
	end_pos = _complete_request.find_first_of(' ', first_pos + 1);
	setUrlStruct(_complete_request.substr(first_pos, end_pos - first_pos));
	_request_method = _complete_request.substr(0, first_pos - 1);

	//making a map of all the request headers
	if (split.size() > 1)
	{
		for (std::size_t i = 1; i < split.size(); i++)
		{
			if (split[i] == "\r\n")
				break ;
			std::size_t pos = split[i].find_first_of(':');
			_headermap[split[i].substr(0, pos)] = split[i].substr(pos + 2, split[i].length());
		}
	}
}

void	RequestHandler::parseHeaderMap(void)
{
	for (t_strmap::const_iterator headerMap_iter = _headermap.begin(); headerMap_iter != _headermap.end(); headerMap_iter++)
	{
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

void	RequestHandler::parseResponse(void)
{
	std::size_t	index;

	void	(RequestHandler::*fptr[])(void) = {
		&RequestHandler::parseHeaderMap,
		&RequestHandler::parseRequestLine,
		&RequestHandler::findServer,
		&RequestHandler::findLocationForUri,
		&RequestHandler::handleRequest,
		NULL
	}; 

	index = 0;
	while (fptr[index] != NULL && !isResponseDone())
	{
		(this->*fptr[index])();
		index++;
	}
}

void	RequestHandler::buildResponsePage(void)
{
	t_strmap					error_pages;
	t_strmap::const_iterator	iter_begin;
	bool						hit;

	hit = false;
	error_pages = _server.getErrorPage();
	for (iter_begin = error_pages.begin(); iter_begin != error_pages.end(); iter_begin++)
	{
		if (_status_line.substr(0, 3) == iter_begin->first)
		{
			std::string file = "/" + _server_start_dir + "/" + _server.getServerRoot() + "/" + iter_begin->second;
			if (checkPath(file) != IS_OTHER)
				openFile(file);
			if (_fd > 0)
				hit = true;
		}
	}
	if (!hit)
		buildDefaultResponsePage();
}

void	RequestHandler::deChunkRequestBody(void)
{
	t_vecstr	lines = splitOnString(_request_body, "\r\n");
	std::string	de_chunked_body;

	for (size_t i = 0; i < lines.size(); i++)
		if (i % 2 != 0)
			de_chunked_body += lines[i];
	
	_request_body = de_chunked_body;
}

void	RequestHandler::checkRequestBodyConditions()
{
	if (_complete_request.find("Transfer-Encoding: chunked") != std::string::npos)
		_is_chunked = true;
	if (_request_header.find("Content-Length:") == std::string::npos)
		_request_body_length = 0;
	else
		_request_body_length = std::stoul(_headermap["Content-Length"]);
}

void	RequestHandler::checkRequestComplete(void)
{
	std::size_t	crlf_pos = _complete_request.find("\r\n\r\n");

	
	if (crlf_pos == std::string::npos)
		return;
	if (!_is_request_header_done)
	{
		_request_header = _complete_request.substr(0, crlf_pos);
		makeHeaderMap();
		_is_request_header_done = true;
		checkRequestBodyConditions();
	}
	if (_is_request_header_done && !_is_chunked && (_complete_request.length() - crlf_pos + 2) >= _request_body_length)
		_is_request_complete = true;
	if (_is_request_header_done && _is_chunked && _complete_request.find("0\r\n\r\n") != std::string::npos)
		_is_request_complete = true;
	if (_is_request_complete)
	{
		_request_body = _complete_request.substr(crlf_pos + 4, std::string::npos);
		if (_is_chunked)
			deChunkRequestBody();
		parseResponse();
		buildResponsePage();
		buildResponseHeader();
	}
}

//TODO why sometimes hangs when resending a form (refresh a page where it was POST) (request gets weird?)
void	RequestHandler::addToRequestMsg(char *msg, int bytes_received)
{
	_complete_request.append(msg, bytes_received);
	if (!isprint(_complete_request[0])) // this is for https and bad requests
	{
		_is_request_complete = true;
		setResponseStatus("400 BAD REQUEST");
		buildResponsePage();
		buildResponseHeader();
		return ;
	}
	checkRequestComplete();
}
