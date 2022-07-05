#include "Server.hpp"

// Constructors
Server::Server()
{
	_client_body_size = 500;
	_server_fd = 0;
	_server_port = 8080;
	_server_name.push_back("test_server");
	_server_ip = "127.0.0.1";
	_root = "var/html/www";
	_error_page = "404.html";
	// _location.insert_or_assign("/", Location());
}

Server::Server(const Server &copy)
{
	(void) copy;
}


// Destructor
Server::~Server()
{
}


// Operators
Server & Server::operator=(const Server &assign)
{
	(void) assign;
	return *this;
}

