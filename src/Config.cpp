#include "Config.hpp"

// Constructors
//TODO in constructor check if file can be opened.
//Map for server, string part is ip + port for easy checking
// for duplicates etc.
Config::Config(std::string config_path)
{
	std::cout << "config path: " << config_path << std::endl;
	//This to add new server to the server map
	_server.insert(std::make_pair("127.0.0.1:8080", new Server()));
	_server.at("127.0.0.1:8080")->setServerIp("127.0.0.1");
	_server.at("127.0.0.1:8080")->setServerPort(8080);
	_server.at("127.0.0.1:8080")->setServerName("testserver");
	_server.at("127.0.0.1:8080")->setServerRoot("var/html/www");
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
