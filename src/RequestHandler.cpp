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

