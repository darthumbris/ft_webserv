#ifndef JSON_HPP
# define JSON_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <map>

# define GREEN   		"\033[32m"
# define RESET_COLOR	"\033[0m"
# define BLUE    		"\033[34m"

class Json;

using jsonObject = std::map<std::string, Json*>;
using jsonList = std::vector<Json*>;

class Json
{
	public:

		// Constructor
		Json();

		// Destructor
		~Json();

		// Member Function
		void print() const;

		enum Token
		{
			STRING,
			NUMBER,
			BOOLEAN,
			ARRAY,
			OBJECT,
			NULL_TYPE
		};

		Token type;
		struct values
		{
			values() {}
			jsonList	list;
			jsonObject	object;
			std::string str;
			int			number;
			bool		boolean;
			~values() {}
		}		values;
};

std::ostream& operator<<(std::ostream& out, const Json::Token& t);

#endif
