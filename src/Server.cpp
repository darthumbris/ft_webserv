# include "../includes/Server.hpp"
# include "../includes/Config.hpp"

// Constructor
Server::Server(): _default_root("/var/www/html/")
{
	if (DEBUG_MODE)
		std::cout << BLUE << "\nAdded a new server." << std::endl;
}

// Destructor
Server::~Server() {}

// Setters
void Server::addServerListen(const Json &json)
{
	for (const Json *x: json.values.list)
	{
		if (x->type != Json::NUMBER)
			throw Config::wrongKey("expected <NUMBER>");
		if (DEBUG_MODE)
			std::cout << BLUE << "Added port: " << x->values.number << " to the server." << RESET_COLOR << std::endl;
		_server_listen.push_back(x->values.number);
	}
}

void Server::addServerName(const Json &json)
{
	for (const auto *x : json.values.list)
	{
		if (x->type != Json::STRING)
			throw Config::wrongKey("expected <STRING>");
		if (DEBUG_MODE)
			std::cout << BLUE << "Added server_name: " << x->values.str << " to the server." << RESET_COLOR << std::endl;
		_server_name.push_back(x->values.str);
	}
}

void Server::setServerClientBodySize(const Json &json)
{
	if (DEBUG_MODE)
		std::cout << BLUE << "Set the client_body_size to: " << json.values.number << " for the server." << RESET_COLOR << std::endl;
	_client_body_size = json.values.number;
}

void Server::setServerErrorPage(const Json &json)
{
	for (const auto *x : json.values.list)
	{
		if (x->type != Json::STRING)
			throw Config::wrongKey("expected <STRING>");
		if (DEBUG_MODE)
			std::cout << BLUE << "Added error_page: " << x->values.str << " to the server" << RESET_COLOR << std::endl;	
		_error_page.push_back(json.values.str);
	}
}

Server::Func Server::setValues(const std::string name, const Json& json)
{
	t_table	map[] =
	{
			{"listen", Json::ARRAY, &Server::addServerListen},
			{"error_page", Json::ARRAY, &Server::setServerErrorPage},
			{"server_name", Json::ARRAY, &Server::addServerName},
			{"client_body_size", Json::NUMBER, &Server::setServerClientBodySize}
	};

	for (const t_table& entry : map)
	{
		if (entry.type == json.type)
		{
			if (entry.key == name)
				return (entry.map_values);
		}
	}
	throw Config::wrongKey("invalid name or key for <" + name);
}

Server & Server::operator=(const Server &assign)
{
	_client_body_size = assign._client_body_size;
	_server_fd = assign._server_fd;
	_server_listen = assign._server_listen;
	_error_page = assign._error_page;
	_server_name = assign._server_name;
	_location = assign._location;
	_server_ip = assign._server_ip;
	_root = assign._root;
	return *this;
}
// Setters
void Server::addServerListen(const Json &json)
{
	for (const Json *x: json.values.list)
	{
		if (x->type != Json::NUMBER)
			throw Config::wrongKey("expected <NUMBER>");
		if (DEBUG_MODE)
			std::cout << BLUE << "Added port: " << x->values.number << " to the server." << RESET_COLOR << std::endl;
		_server_listen.push_back(x->values.number);
	}
}

void	Server::setServerSocket(int server_socket)
{
	this->_server_fd.push_back(server_socket);
}


// Getters
int Server::getClientBodySize() const
{
	return _client_body_size;
}

const std::string&		Server::getServerIp() const
{
	return this->_server_ip;
}

const std::vector<int> &Server::getServerPort() const
{
	return _server_listen;
}


const t_vecstr &Server::getErrorPage() const
{
	return _error_page;
}

t_locmap	Server::getLocationMap() const
{
	return this->_location;
}

void Server::setServerErrorPage(const Json &json)
{
	for (const auto &x : json.values.object)
	{
		if (x.second->type != Json::STRING)
			throw Config::wrongKey("Expected STRING type");
		if (_error_page.find(x.first) != _error_page.end())
			throw Config::wrongKey("error_code duplicate");
		std::cout << BLUE << "For error code: " << x.first << " set the file to: " << x.second->values.str << RESET_COLOR << std::endl; 
		_error_page.emplace(x.first, x.second->values.str);
	}
}

void Server::setServerRoot(const Json& json)
{
	_root = json.values.str;
	if (DEBUG_MODE)
		std::cout << GREEN << "set the root_folder: " << _root << " for this location" <<  RESET_COLOR << std::endl;
}

Server::Func Server::setValues(const std::string name, const Json& json)
{
	t_table	map[] =
	{
			{"listen", Json::ARRAY, &Server::addServerListen},
			{"error_page", Json::OBJECT, &Server::setServerErrorPage},
			{"server_name", Json::ARRAY, &Server::addServerName},
			{"client_body_size", Json::NUMBER, &Server::setServerClientBodySize},
			{"root", Json::STRING, &Server::setServerRoot}
	};

	for (const t_table& entry : map)
	{
		if (entry.type == json.type)
		{
			if (entry.key == name)
				return (entry.map_values);
		}
	}
	throw Config::wrongKey("invalid name or key for <" + name);
}

void	Server::setServerSocket(int server_socket)
{
	this->_server_fd.push_back(server_socket);
}

// Getters

const std::string		&Server::getDefaultRoot(void) const
{
	return _default_root;
}

int Server::getClientBodySize() const
{
	return _client_body_size;
}

const std::string&		Server::getServerIp() const
{
	for (std::size_t i = 0; i < _server_listen.size(); i++)
	{
		if (_server_listen[i] == port)
		{
			// std::cout << "looking at port:" << port << " and url: " << url << std::endl;
			if (_location.find(url) != _location.end())
				return (_location.find(url)->second);
		}
	}
	return NULL;
}

const t_vecstr &Server::getServerNames() const
{
	return _server_name;
}

Location *Server::getLocation(int port, std::string url) const
{
	for (std::size_t i = 0; i < _server_listen.size(); i++)
	{
		if (_server_listen[i] == port)
		{
			// std::cout << "looking at port:" << port << " and url: " << url << std::endl;
			if (_location.find(url) != _location.end())
				return (_location.find(url)->second);
		}
	}
	return NULL;
}


const std::string		&Server::getServerRoot(void) const
{
	return _root == "" ? _default_root : _root;
}

// Member Function
void	Server::addLocationToServer(std::string location_dir, Location *loc)
{
	this->_location.insert(std::make_pair(location_dir, loc));
}
