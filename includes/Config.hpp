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

		// Getters
		std::map<std::string, Server*>	getServerMap() const;

		// Member Functions

	private:
		std::map<std::string, Server*>	_server;
};

#endif
