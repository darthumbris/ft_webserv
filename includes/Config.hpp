#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Json.hpp"

# include "Location.hpp"
# include "Server.hpp"

class Config
{
	public:
		// Constructors
		Config();
		Config(const Config &copy);
		Config(const Json* json);

		// Destructor
		~Config();

		// Setters
		void addServer(const Json* json);
		void setServers(const Json* json);
		void checkValidServer(const Json &json);

		// Getters
		t_servmap	getServerMap() const;
		Server		getLastServer();

		// Exception
		class wrongKey: public std::exception
		{
			private:
				const char* _msg;
			public:
				wrongKey(const char* msg) : _msg(msg) {}
				virtual const char* what() const throw() {
					return _msg;
				}
		};

	private:

		t_servmap	_servers;
};

#endif
