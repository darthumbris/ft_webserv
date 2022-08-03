#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Server.hpp"

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
		Server		*getLastServer();

		// Member Functions
		void	addServer(std::string ip, std::string port);
		void	addLocation(std::string location_dir);

	private:
		t_servmap	_server;

};

#endif
