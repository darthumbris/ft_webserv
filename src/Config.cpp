#include "../includes/Config.hpp"
#include "../includes/Location.hpp"

Config::Config() {}

Config::Config(const Json *json) {
	set_servers(json);
	for (auto x : _servers) {
		std::cout << "testing lol " << x.getClientBodySize() << std::endl;
		for (auto x : server->getLocationMap()) {
			std::cout << x.first << std::endl;
		}
	}
}

void Config::set_servers(const Json *json) {
	for (const auto &x: json->values.object) {
		set_server_name(x.first, *x.second);
	}
	if (loc) {
		std::cout << "RET URL: " << loc->getReturnUrl() << std::endl;
	}
}

void Config::set_server_name(const std::string name, const Json &json) {
	(void)name;
	if (json.type != Json::ARRAY) {
		throw wrongKey("expected an <" + loc->getEnumValue(Json::ARRAY) + "> and got " + loc->getEnumValue(json.type));
	}
	for (const Json *x: json.values.list) {
		test(x);
	}
}

void Config::test(const Json *json) {
	server = new Server();
	for (const auto &x: json->values.object) {
		if(x.first.find("location") != std::string::npos) {
			loc = new Location();
			loc->get_location(x.first, *x.second);
			server->getLocationMap().emplace(x.first, loc);
		} else {
			Server::Func f = server->set_values(x.first, *x.second);
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