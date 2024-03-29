# include "../includes/Server.hpp"
# include "../includes/Config.hpp"

// Constructor
Server::Server(): _default_root("/var/www/html/")
{
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

// Destructor
Server::~Server() {}

// Setters

void Server::addServerListen(const Json &json)
{
	for (jsonList::const_iterator x = json.values.list.begin(); x != json.values.list.end(); x++)
	{
		if ((*x)->type != Json::NUMBER)
			throw Config::wrongKey("expected <NUMBER>");
		if (DEBUG_MODE)
			std::cout << BLUE << "Added port: " << (*x)->values.number << " to the server." << RESET_COLOR << std::endl;
		_server_listen.push_back((*x)->values.number);
	}
}

void Server::addServerName(const Json &json)
{
	for (jsonList::const_iterator x = json.values.list.begin(); x != json.values.list.end(); x++)
	{
		if ((*x)->type != Json::STRING)
			throw Config::wrongKey("expected <STRING>");
		if (DEBUG_MODE)
			std::cout << BLUE << "Added server_name: " << (*x)->values.str << " to the server." << RESET_COLOR << std::endl;
		_server_name.push_back((*x)->values.str);
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
	for (jsonObject::const_iterator x = json.values.object.begin(); x != json.values.object.end(); x++)
	{
		if (x->second->type != Json::STRING)
			throw Config::wrongKey("Expected STRING type");
		if (_error_page.find(x->first) != _error_page.end())
			throw Config::wrongKey("error_code duplicate");
		if (!std::isdigit(x->first[0]) || std::stoi(x->first) < 100 || std::stoi(x->first) > 599 || x->first.length() != 3)
			throw Config::wrongKey(("error_code not in range 100-599 got: " + x->first + ".").c_str());
		if (x->second->values.str.length() == 0)
			throw Config::wrongKey("error_page is empty string.");
		if (DEBUG_MODE)
			std::cout << BLUE << "For error code: " << x->first << " set the file to: " << x->second->values.str << RESET_COLOR << std::endl; 
		_error_page.insert(std::make_pair(x->first, x->second->values.str));
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
	t_table	map[5] =
	{
			{"listen", Json::ARRAY, &Server::addServerListen},
			{"error_page", Json::OBJECT, &Server::setServerErrorPage},
			{"server_name", Json::ARRAY, &Server::addServerName},
			{"client_body_size", Json::NUMBER, &Server::setServerClientBodySize},
			{"root", Json::STRING, &Server::setServerRoot}
	};

	for (size_t i = 0; i < 5; i++)
	{
		if (map[i].type == json.type)
		{
			if (map[i].key == name)
				return (map[i].map_values);
		}
	}
	throw Config::wrongKey(("invalid name or key for <" + name).c_str());
}

void	Server::setServerSocket(int server_socket)
{
	this->_server_fd.push_back(server_socket);
}


// Getters
bool	Server::hasPort(int port) const
{
	for (t_vecint::const_iterator it = _server_listen.begin(); it != _server_listen.end(); it++)
		if (*it == port)
			return true;
	return false;
}

bool	Server::hasMatchingHost(std::string& host) const
{
	for (t_vecstr::const_iterator it = _server_name.begin(); it != _server_name.end(); it++)
		if (*it == host)
			return true;
	return false;
}

int Server::getClientBodySize() const
{
	return _client_body_size;
}

const std::string&		Server::getServerIp() const
{
	return this->_server_ip;
}

const t_vecint &Server::getServerPort() const
{
	return _server_listen;
}


const t_strmap &Server::getErrorPage() const
{
	return _error_page;
}

const t_locmap	&Server::getLocationMap() const
{
	return this->_location;
}

// Getters

const std::string		&Server::getDefaultRoot(void) const
{
	return _default_root;
}

const t_vecstr &Server::getServerNames() const
{
	return _server_name;
}

const std::string		&Server::getServerRoot(void) const
{
	return _root == "" ? _default_root : _root;
}


void	Server::addLocationToServer(std::string location_dir, Location *loc)
{
	this->_location.insert(std::make_pair(location_dir, loc));
}

