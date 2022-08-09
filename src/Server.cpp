#include "Server.hpp"

// Constructors
Server::Server() : _server_ip("127.0.0.1")
{
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

std::vector<int>	Server::getServerPort() const
{
	return this->_server_port;
}

t_vecstr	Server::getServerNames() const
{
	return this->_server_name;
}

t_locmap	Server::getLocationMap() const
{
	return this->_location;
}

t_vecstr	Server::getErrorPage() const
{
	return this->_error_page;
}

int	Server::getClientBodySize() const
{
	return this->_client_body_size;
}

std::vector<int>	Server::getServerSocket() const
{
	return this->_server_fd;
}

Location *Server::getLocation(int port, std::string url) const
{
	for (std::size_t i = 0; i < _server_port.size(); i++)
	{
		if (_server_port[i] == port)
		{
			if (_location.find(url) != _location.end())
				return (_location.find(url)->second);
		}
	}
	return NULL;
}

// Setters
void	Server::setServerIp(const std::string& ip)
{
	this->_server_ip = ip;
}

void	Server::addServerPort(int port)
{
	this->_server_port.push_back(port);
}

void	Server::addServerName(std::string server_name)
{
	this->_server_name.push_back(server_name);
}

void	Server::setServerSocket(int server_socket)
{
	this->_server_fd.push_back(server_socket);
}

void	Server::setServerClientBodySize(int size)
{
	this->_client_body_size = size;
}

void	Server::setServerErrorPage(std::string error_page)
{
	this->_error_page.push_back(error_page);
}

// Member Functions
void	Server::addLocationToServer(std::string location_dir)
{
	//TODO check for error inserting? (for example two same directories)
	this->_location.insert(std::make_pair(location_dir, new Location()));
}
