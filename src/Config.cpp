#include "../includes/Config.hpp"

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
		checkValidServer(*x.second);
}

void Config::checkValidServer(const Json &json)
{
	if (json.type != Json::ARRAY)
	{
		std::string	error = "expected an <ARRAY> and got " + Location().getEnumValue(json.type);
		throw wrongKey(error.c_str());
	}
	for (const Json *x: json.values.list)
		addServer(x);
}

void Config::addServer(const Json *json)
{
	Server		server;

	if (DEBUG_MODE)
		std::cout << BLUE << "\nAdded a new server." << std::endl;
	for (const auto &x: json->values.object)
	{
		if(x.first.find("location") != std::string::npos)
		{
			Location	*loc = new Location();

			loc->ParseLocation(x.first, *x.second);
			server.addLocationToServer(loc->getPath(x.first), loc);
		}
		else
		{
			Server::Func f = server.setValues(x.first, *x.second);
			(server.*f)(*x.second);
		}
	}
	_servers.push_back(server);
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
