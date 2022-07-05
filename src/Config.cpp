#include "Config.hpp"

// Constructors
//TODO in constructor check if file can be opened.
Config::Config(std::string config_path)
{
	(void)config_path;
	// _server.;
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