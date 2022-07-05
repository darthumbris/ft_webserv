#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <map>
# include "Server.hpp"

class Server;

class Config
{
	public:
		// Constructors
		Config(std::string config_path);
		Config(const Config &copy);
		
		// Destructor
		~Config();
		
		// Operators
		Config & operator=(const Config &assign);
		
	private:
		std::map<int, Server>	_server;
};

#endif
