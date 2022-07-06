#include "Server.hpp"

// Constructors
//TODO make a proper constructor for server instead this hardcoded stuff
Server::Server()
{
	_client_body_size = 500;
	_server_fd = 0;
	_server_port = 8080;
	_server_name.push_back("test_server");
	_server_ip = "127.0.0.1";
	_error_page = "404.html";

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
	return this->_server_ip;
}

int	Server::getServerPort() const
{
	return this->_server_port;
}

std::vector<std::string>	Server::getServerNames() const
{
	return this->_server_name;
}

std::map<std::string, Location*>	Server::getLocationMap() const
{
	return this->_location;
}

std::string	Server::getErrorPage() const
{
	return this->_error_page;
}

int	Server::getClientBodySize() const
{
	return this->_client_body_size;
}

int	Server::getServerSocket() const
{
	return this->_server_fd;
}

// Setters
void	Server::setServerIp(std::string ip)
{
	this->_server_ip = ip;
}

void	Server::setServerPort(int port)
{
	this->_server_port = port;
}

void	Server::setServerName(std::string server_name)
{
	this->_server_name.push_back(server_name);
}

void	Server::setServerSocket(int server_socket)
{
	this->_server_fd = server_socket;
}

void	Server::setServerClientBodySize(int size)
{
	this->_client_body_size = size;
}

void	Server::setServerErrorPage(std::string error_page)
{
	this->_error_page = error_page;
}

// Member Functions
void	Server::addLocationToServer(std::string location_dir)
{
	this->_location.insert(std::make_pair(location_dir, new Location()));
}
