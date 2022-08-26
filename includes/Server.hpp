#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Location.hpp"

class Server;
class Location;

using t_locmap = std::map<std::string, Location*>;
using t_servmap =  std::vector<Server>;
using t_vecstr = std::vector<std::string>;
using t_strmap = std::map<std::string, std::string>;

class Server
{
	public:
		// Constructors
		Server();

		// Destructor
		~Server();

		// Overloads
		Server & operator=(const Server &assign);
		// Getters
		//	std::vector<int>	getServerSocket() const;

		const std::string		&getDefaultRoot(void) const;
		const std::string&		getServerIp() const;
		int						getClientBodySize() const;
		const std::vector<int>&	getServerPort() const;
		const t_strmap&			getErrorPage() const;
		const t_vecstr&			getServerNames() const;
		const t_locmap&			getLocationMap() const;
		// Location				*getLocation(int port, std::string url) const;

		const std::string		&getServerRoot(void) const;

		// Setters
		void	setServerRoot(const Json& json);
		void	setServerIp(const Json& json);
		void	addServerListen(const Json& json);
		void	addServerName(const Json& json);
		void	setServerClientBodySize(const Json& json);
		void	setServerErrorPage(const Json& json);
		void	setServerSocket(int server_socket);

		typedef void (Server::* Func )(const Json&);
		//Table to check which function to jump in
		typedef struct s_table
		{
			std::string	key;
			Json::Token	type;
			Func		map_values;
		}	t_table;

		// Member Functions
		void	addLocationToServer(std::string location_dir, Location *loc);
		Func setValues(const std::string name, const Json& json);
		
	private:
		int					_client_body_size;
		std::vector<int>	_server_fd;
		std::vector<int>	_server_listen;
		t_strmap			_error_page;
		t_vecstr			_server_name;
		t_locmap			_location;
		std::string			_server_ip;
		std::string			_root;
		const std::string	_default_root = "/var/www/html";
};

#endif
