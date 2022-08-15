# include "../includes/Server.hpp"
# include "../includes/Config.hpp"

Server::Server() {}

void Server::addServerListen(const Json &json) {
	for (const Json *x: json.values.list) {
		if (x->type != Json::NUMBER) {
			throw Config::wrongKey("expected <NUMBER>");
		}
		_server_listen.push_back(x->values.number);
	}
}

void Server::addServerName(const Json &json) {
	for (const auto *x : json.values.list) {
		if (x->type != Json::STRING) {
			throw Config::wrongKey("expected <STRING>");
		}
		_server_name.push_back(x->values.str);
	}
}

void Server::setServerClientBodySize(const Json &json) {
	_client_body_size = json.values.number;
}

void Server::setServerErrorPage(const Json &json) {
	for (const auto *x : json.values.list) {
		if (x->type != Json::STRING) {
			throw Config::wrongKey("expected <STRING>");
		}
		_error_page.push_back(json.values.str);
	}
}

Server::Func Server::set_values(const std::string name, const Json& json) {
	t_table	map[] = {
			{"listen", Json::ARRAY, &Server::addServerListen},
			{"error_page", Json::ARRAY, &Server::setServerErrorPage},
			{"server_name", Json::ARRAY, &Server::addServerName},
			{"client_body_size", Json::NUMBER, &Server::setServerClientBodySize}
	};

	for (const t_table& entry : map) {
		if (entry.type == json.type) {
			if (entry.key == name) {
				return (entry.map_values);
			}
		}
	}
	throw Config::wrongKey("invalid name or key for <" + name);
}

void	Server::setServerSocket(int server_socket)
{
	this->_server_fd.push_back(server_socket);
}

int Server::getClientBodySize() const {
	return _client_body_size;
}

const std::string&		Server::getServerIp() const
{
	return this->_server_ip;
}

const std::vector<int> &Server::getServerPort() const {
	return _server_listen;
}


const t_vecstr &Server::getErrorPage() const {
	return _error_page;
}

const t_vecstr &Server::getServerNames() const {
	return _server_name;
}

t_locmap	Server::getLocationMap() const
{
	return this->_location;
}

Location *Server::getLocation(int port, std::string url) const
{
	for (std::size_t i = 0; i < _server_listen.size(); i++)
	{
		if (_server_listen[i] == port)
		{
			if (_location.find(url) != _location.end())
				return (_location.find(url)->second);
		}
	}
	return NULL;
}

Server::~Server() {}

