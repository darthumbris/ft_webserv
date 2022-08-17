#include "../includes/Config.hpp"
#include "../includes/Location.hpp"

// Constructors
Config::Config() {}

Config::Config(const Json *json)
{
	setServers(json);
}

// Destructor
Config::~Config()
{
}

// Setters
void Config::setServers(const Json *json)
{
	for (const auto &x: json->values.object)
		setServerName(x.first, *x.second);
}

void Config::setServerName(const std::string name, const Json &json)
{
	(void)name;
	Location	loc;
	if (json.type != Json::ARRAY)
		throw wrongKey("expected an <" + loc.getEnumValue(Json::ARRAY) + "> and got " + loc.getEnumValue(json.type));
	for (const Json *x: json.values.list)
		addServer(x);
}

void Config::addServer(const Json *json)
{
	Server		*server = new Server();

	for (const auto &x: json->values.object)
	{
		if(x.first.find("location") != std::string::npos)
		{
			Location	*loc = new Location();

			loc->ParseLocation(x.first, *x.second);
			server->addLocationToServer(loc->getPath(x.first), loc);
		}
		else
		{
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
	return _servers.back();
}
