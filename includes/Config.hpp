#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <fcntl.h>
# include <sys/event.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <vector>

class Config
{
	public:
		// Constructors
		Config(std::string config_path);
		// Config();
		Config(const Config &copy);
		
		// Destructor
		~Config();
		
		// Operators
		Config & operator=(const Config &assign);
		
	private:
		struct sockaddr_in	_address;
		int					_port;
		std::string			_host_name;
		std::string			_root;
};

#endif
