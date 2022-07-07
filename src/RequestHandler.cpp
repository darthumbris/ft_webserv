#include "RequestHandler.hpp"

// Constructors
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
std::string	RequestHandler::getResponse() const
{
	return this->_response;
}

// Setters
void	RequestHandler::setRequestMsg(std::string msg) 
{
	_msg = msg;
	std::cout << msg;
	//TODO make sure to parse this message. 
	//TODO might need a need to see if the msg is done being received?
	// for request headers they always end with \r\n\r\n (section 4.1 of RFC 2616)
	// We can use that to check if the full message has been received.
	// if it has been fully received set a bool _is_fully_read or something to true?
	// might also work for sending the response?
	if (msg.find("\r\n\r\n"))
		std::cout << "end of request header" << std::endl;
}
