#include "Config.hpp"

// Constructors
Config::Config(std::string config_path)
{
	(void)config_path;
	_port = 8080;
	_host_name = "hello";
	_root = "/";
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	_address.sin_port = htons(_port);
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

