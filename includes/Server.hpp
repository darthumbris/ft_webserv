#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Location.hpp"

class Server;

using t_locmap = std::map<std::string, Location*>;
using t_servmap =  std::vector<Server*>;
using t_vecstr = std::vector<std::string>;

//TODO IP should probably always be 127.0.0.1 then at least one port should be set per server. A server might also have servernames.

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
		std::vector<int>	getServerPort() const;
		int					getClientBodySize() const;
		std::vector<int>	getServerSocket() const;
		std::string			getServerIp() const;
		t_vecstr			getErrorPage() const;
		t_vecstr			getServerNames() const;
		t_locmap			getLocationMap() const;
		std::string			getServerRoot() const;
		Location			*getLocation(int port, std::string url) const;

		// Setters
		void	setServerIp(const std::string& ip);
		void	addServerPort(int port);
		void	addServerName(std::string server_name);
		void	setServerRoot(std::string root);
		void	setServerSocket(int server_socket);
		void	setServerClientBodySize(int size);
		void	setServerErrorPage(std::string error_page);

		// Member Functions
		void	addLocationToServer(std::string location_dir);
		
	private:
		int					_client_body_size;
		std::vector<int>	_server_fd;
		std::vector<int>	_server_port;
		std::string			_server_ip;
		t_vecstr			_error_page;
		t_vecstr			_server_name;
		t_locmap			_location;
		std::string			_root;
};

#endif
