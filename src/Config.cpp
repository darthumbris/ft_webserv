#include "Config.hpp"

// Constructors
//TODO in constructor check if file can be opened.
//Map for server, string part is ip + port for easy checking
// for duplicates etc.
Config::Config(std::string config_path)
{
	std::cout << "config path: " << config_path << std::endl;
	//This to add new server to the server map
	if (_server.find("127.0.0.1:8080") != _server.end())
		std::cout << "Error: duplicate server in config" << std::endl;
	_server.insert(std::make_pair("127.0.0.1:8080", new Server()));
	_server.at("127.0.0.1:8080")->setServerIp("127.0.0.1");
	_server.at("127.0.0.1:8080")->setServerPort(8080);
	_server.at("127.0.0.1:8080")->setServerName("testserver");
	if (_server.at("127.0.0.1:8080")->getLocationMap().size() && _server.at("127.0.0.1:8080")->getLocationMap().find("fsda/dsafasa") != _server.at("127.0.0.1:8080")->getLocationMap().end())
		std::cout << "Error: duplicate location in config" << std::endl;
	_server.at("127.0.0.1:8080")->addLocationToServer("/");
}

Config::Config(const Config &copy)
{
	(void) copy;
}


// Destructor
Config::~Config()
{
}


// Operators
Config & Config::operator=(const Config &assign)
{
	(void) assign;
	return *this;
}

//TODO make proper getter/setter and member functions.

//TODO make a parseconfig function which goes through the config
// and adds new servers and locations (to the corresponding servers)

// Getters
std::map<std::string, Server*>	Config::getServerMap() const
{
	return this->_server;
}

// Member Functions
