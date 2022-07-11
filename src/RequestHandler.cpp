#include "RequestHandler.hpp"

/*TODO need to have a check here for post to check how big the upload is
// and compare it to the client_body_size variable of the server
*/

// Constructors
RequestHandler::RequestHandler(Server *server) : _server(server), _is_request_complete(false)
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

// Setters
void	RequestHandler::addToRequestMsg(const std::string &msg)
{
	_complete_request += msg;
	if (_complete_request.find("\r\n\r\n"))
	{
		_is_request_complete = true;
		if (_complete_request.find("GET / HTTP/1.1"))
		{
			// std::cout << "Get response" << std::endl;
			_response.append("HTTP/1.1 200 OK\nContent-Type: text/html\n");
			_response.append("Content-Length: 100\n\n");
			_response.append("<!DOCTYPE html>\n<html>\n<body>\n\n<h1>My First Heading</h1>\n");
			_response.append("<p>My first paragraph.</p>\n\n</body>\n</html>\r\n\r\n");
		}
		std::cout << _complete_request;
		_complete_request.erase();
	}
	//TODO make sure to parse this message. 
	//TODO might need a need to see if the msg is done being received?
	// for request headers they always end with \r\n\r\n (section 4.1 of RFC 2616)
	// We can use that to check if the full message has been received.
	// if it has been fully received set a bool _is_fully_read or something to true?
	// might also work for sending the response?
	// if (msg.find("\r\n\r\n"))
		// std::cout << "end of request header" << std::endl;
}
