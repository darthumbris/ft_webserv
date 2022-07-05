#include "Config.hpp"

// Constructors
Config::Config(std::string config_path)
{
	(void)config_path;
	(void)_server;
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

