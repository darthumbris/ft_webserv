#include "Config.hpp"

// Constructors
//TODO in constructor check if file can be opened.
// TODO maybe needs to be a check if there are no duplicate server_names?
Config::Config(std::string config_path)
{
	std::cout << "config filename: " << config_path << std::endl;
	//After this should parse the config


	//Examples for the parsing:
	// Config parsed new server with listen 127.0.0.1 8080
	addServer("127.0.0.1", "4242");
	// Config sees server_name test.com
	getLastServer()->addServerName("test.com");
	getLastServer()->addServerName("test2.com");
	getLastServer()->addServerName("test3.com");
	getLastServer()->addServerPort(4343);

	// Config parsed new location with dir: "/images"
	addLocation("/images");
	addLocation("/");
	// Setting autoindex for location / to true (default is off)
	getLastServer()->getLocationMap()["/"]->setAutoIndex(true);

	// Config parsed new server with listen 0.0.0.0 4242
	addServer("0.0.0.0", "7575");
	getLastServer()->addServerPort(4343);
	getLastServer()->addServerName("different.com");
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

//TODO make a parseconfig function which goes through the config
// and adds new servers and locations (to the corresponding servers) (for ABBA)

// Getters
t_servmap	Config::getServerMap() const
{
	return this->_server;
}

Server	*Config::getLastServer()
{
	return (_server.back());
}



// Member Functions
void	Config::addServer(std::string ip, std::string port)
{
	//Ip can probably just be always 127.0.0.1 ?
	std::string	server_key = ip + ":" +  port;
	_server.push_back(new Server());
	_server.back()->setServerIp(ip);
	_server.back()->addServerPort(std::stoi(port));
}

void	Config::addLocation(std::string location_dir)
{
	Server		*server;
	t_locmap	location;

	server = getLastServer();
	location = server->getLocationMap();
	if (location.size() && location.find(location_dir) != location.end())
		std::cout << "Error: duplicate location in config" << std::endl; // Should throw ?, can of course be a different server with same location
	server->addLocationToServer(location_dir);
}
