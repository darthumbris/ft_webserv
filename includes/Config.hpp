#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <map>
# include "Server.hpp"

class	Server;
class	Location;

typedef std::map<std::string, Location*>	t_locmap;
typedef std::map<std::string, Server*>		t_servmap;

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
		t_servmap	getServerMap() const;
		Server	*getLastServer();

		// Member Functions
		void	addServer(std::string ip, std::string port);
		void	addLocation(std::string location_dir);

	private:
		t_servmap	_server;

};

#endif
