#include "RequestHandler.hpp"

/*TODO need to have a check here for post to check how big the upload is
// and compare it to the client_body_size variable of the server
*/

// Constructors
RequestHandler::RequestHandler(Server *server) : _server(server), _is_request_complete(false), _has_remaining_request(false)
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
	return this->_response;
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

// Setters
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
		if (_complete_request.find("GET / HTTP/1.1") != std::string::npos) // just for testing
		{
			std::cout << "Get response" << std::endl;
			_response.append("HTTP/1.1 200 OK\nContent-Type: text/html\n");
			_response.append("Content-Length: 190\n\n");
			_response.append("<!DOCTYPE html>\n<html>\n");
			_response.append("<head>\n<title>My Page Title</title>\n");
			_response.append("<link rel=\"icon\" href=\"data:;base64,=\">\n");
			_response.append("</head>\n<body>\n\n<h1>This is a Heading</h1>\n");
			_response.append("<p>This is a paragraph.</p>\n\n</body>\n</html>\r\n\r\n");
		}
		else if (_complete_request.find("GET /test.png HTTP/1.1") != std::string::npos) // testing how image things are handled
		{
			_response.append("HTTP/1.1 200 ok\nContent-Type: image/png; charset=utf-8\r\n");
			_response.append("Connection: close\r\n\r\n");
			// _response.append
			//TODO see how to open file etc and load it in here?
		}
		std::cout << _complete_request;  // just for testing
		if (size_req - crlf_pos != 4) // in case recv has gotten more than a single HTTP request
		{
			_has_remaining_request = true;
			_remaining_request = _complete_request.substr(crlf_pos, size_req - crlf_pos);
			std::cout << "\nMore than one packet in the receive msg" << std::endl; // just for testing
			std::cout << _remaining_request << std::endl; // just for testing
		}
	}
	// std::cout << _complete_request;
	//TODO make sure to parse this message. 
	//TODO might need a need to see if the msg is done being received?
	// for request headers they always end with \r\n\r\n (section 4.1 of RFC 2616)
	// We can use that to check if the full message has been received.
	// if it has been fully received set a bool _is_request_complete or something to true?
	// might also work for sending the response?
}
