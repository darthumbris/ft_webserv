#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../json/includes/json.hpp"

# include "Location.hpp"
# include "Server.hpp"

class Config {

//	FOR TESTING
private:
	t_servmap	_server;

public:
	Location	*loc = NULL;
	Server		*server = NULL;
	std::vector<Location> location;

public:
	// Constructors
	Config();
	Config(const Config &copy);
	Config(const Json* json);
	~Config();

	void test(const Json* json);
	void set_servers(const Json* json);
	void set_server_name(const std::string name, const Json &json);

	void	set_listen(const Json& json);

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