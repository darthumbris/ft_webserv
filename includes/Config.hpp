#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Json.hpp"

# include "Location.hpp"
# include "Server.hpp"

class Config {

//	FOR TESTING
private:
	t_servmap	_servers;

// public:
	Location	*loc = NULL;
	Server		*server = NULL;
	// std::vector<Location> location;

public:
	// Constructors
	Config();
	Config(const Config &copy);
	Config(const Json* json);
	~Config();

	void addServer(const Json* json);
	void setServers(const Json* json);
	void setServerName(const std::string name, const Json &json);

	t_servmap	getServerMap() const;
	Server		getLastServer();


	// void	setListenPort(const Json& json);

	class wrongKey: public std::exception {
	private:
		std::string _msg;
	public:
		wrongKey(const std::string& msg) : _msg(msg) {}
		virtual const char* what() const throw() {
			return _msg.c_str();
		}
	};

};

#endif