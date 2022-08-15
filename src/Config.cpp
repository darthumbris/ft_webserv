#include "../includes/Config.hpp"
#include "../includes/Location.hpp"

Config::Config() {}

Config::Config(const Json *json) {
	setServers(json);
	for (auto x : _servers) {
		std::cout << "Client_body_size: " << x.getClientBodySize() << std::endl;
		for (auto x : server->getLocationMap()) {
			std::cout << x.first << std::endl;
		}
	}
}

void Config::setServers(const Json *json) {
	for (const auto &x: json->values.object) {
		setServerName(x.first, *x.second);
	}
	if (loc) {
		std::cout << "RET URL: " << loc->getReturnUrl() << std::endl;
	}
}

void Config::setServerName(const std::string name, const Json &json) {
	(void)name;
	if (json.type != Json::ARRAY) {
		throw wrongKey("expected an <" + loc->getEnumValue(Json::ARRAY) + "> and got " + loc->getEnumValue(json.type));
	}
	for (const Json *x: json.values.list) {
		addServer(x);
	}
}

void Config::addServer(const Json *json) {
	server = new Server();
	for (const auto &x: json->values.object) {
		if(x.first.find("location") != std::string::npos) {
			loc = new Location();
			loc->getParsedLocation(x.first, *x.second);
			std::cout << "location dir: " << x.first << std::endl;
			std::cout << "locdir size: " << x.first.size() << std::endl;
			server->addLocationToServer(loc->getPath(x.first), loc);
		} else {
			Server::Func f = server->setValues(x.first, *x.second);
			(*server.*f)(*x.second);
		}
	}
	_servers.push_back(*server);
}

// Getters
t_servmap	Config::getServerMap() const
{
	return this->_servers;
}

Server	Config::getLastServer()
{
	return (_servers.back());
}

Config::~Config()
{
	delete loc;
}