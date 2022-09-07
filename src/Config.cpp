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
	for (jsonObject::const_iterator x = json->values.object.begin(); x != json->values.object.end(); x++)
		checkValidServer(*x->second);
}

void Config::checkValidServer(const Json &json)
{
	if (json.type != Json::ARRAY)
	{
		std::string	error = "expected an <ARRAY> and got " + Location().getEnumValue(json.type);
		throw wrongKey(error.c_str());
	}
	for (jsonList::const_iterator x = json.values.list.begin(); x != json.values.list.end(); x++)
		addServer(*x);
}

void Config::addServer(const Json *json)
{
	Server		server;

	if (DEBUG_MODE)
		std::cout << BLUE << "\nAdded a new server." << std::endl;
	for (jsonObject::const_iterator x = json->values.object.begin(); x != json->values.object.end(); x++)
	{
		if(x->first.find("location") != std::string::npos)
		{
			Location	*loc = new Location();

			loc->ParseLocation(x->first, *x->second);
			server.addLocationToServer(loc->getPath(x->first), loc);
		}
		else
		{
			Server::Func f = server.setValues(x->first, *x->second);
			(server.*f)(*x->second);
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
