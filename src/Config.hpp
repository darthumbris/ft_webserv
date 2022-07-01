#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <fcntl.h>
# include <sys/event.h>
# include <sys/socket.h>
# include <netdb.h>

class Config
{
	public:
		// Constructors
		Config();
		Config(const Config &copy);
		
		// Destructor
		~Config();
		
		// Operators
		Config & operator=(const Config &assign);
		
	private:
		struct sockaddr_in	address;
		int					_port;
		std::string			_host_name;
		std::string			_root;
};

#endif