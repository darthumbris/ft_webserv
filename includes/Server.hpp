#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include "Location.hpp"

class Location;

class Server
{
	public:
		// Constructors
		Server();
		Server(const Server &copy);
		
		// Destructor
		~Server();
		
		// Operators
		Server & operator=(const Server &assign);

		// Getters
		std::string					getServerIp() const;
		int							getServerPort() const;
		std::vector<std::string>	getServerNames() const;
		std::string					getServerRoot() const;

		// Setters
		void	setServerIp(std::string ip);
		void	setServerPort(int port);
		void	setServerName(std::string server_name);
		void	setServerRoot(std::string root);

		// Member Functions
		
		
	private:
		int								_client_body_size;
		int								_server_fd;
		int								_server_port;
		std::vector<std::string>		_server_name;
		std::string						_server_ip;
		std::string						_error_page;
		std::map<std::string, Location>	_location;
};

#endif
