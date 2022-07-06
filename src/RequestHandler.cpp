#include "RequestHandler.hpp"

// Constructors
RequestHandler::RequestHandler(Server *server) : _server(server)
{
	(void)_server;
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


// Member Functions
void	RequestHandler::addRequestMsg(std::string msg) 
{
	_msg = msg;
	std::cout << msg;
	//TODO make sure to parse this message. 
	//TODO might need a need to see if the msg is done being received?
}
