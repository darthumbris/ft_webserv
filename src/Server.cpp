#include "Server.hpp"

// Constructors
//TODO make a proper constructor for server
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

// Getters
std::string	Server::getServerIp() const
{
	std::cout << "returning this->server_ip: " << this->_server_ip << std::endl;
	return this->_server_ip;
}

int			Server::getServerPort() const
{
	return this->_server_port;
}

std::vector<std::string>	Server::getServerNames() const
{
	return this->_server_name;
}

std::string	Server::getServerRoot() const
{
	return this->_root;
}

// Setters
void	Server::setServerIp(std::string ip)
{
	this->_server_ip = ip;
	std::cout << "set server ip to " << ip << std::endl;
	std::cout << "this->_server_ip is: " << this->_server_ip << std::endl;
}

void	Server::setServerPort(int port)
{
	this->_server_port = port;
}

void	Server::setServerName(std::string server_name)
{
	this->_server_name.push_back(server_name);
}

void	Server::setServerRoot(std::string root)
{
	this->_root = root;
}


//TODO make getter and setter etc functions for server class
