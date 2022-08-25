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

# ifndef DEBUG_MODE
#  define DEBUG_MODE	0
# endif

class Json;

using jsonObject = std::map<std::string, Json*>;
using jsonList = std::vector<Json*>;

class Json
{
	public:

		// Constructor
		Json();
		Json(jsonList&& list) : values(std::move(list)) {}
		Json(jsonObject&& object) : values(std::move(object)) {}
		Json(const std::string& str) : values(std::move(str)) {}
		Json(int number) : values(number) {}
		Json(bool boolean) : values(boolean) {}

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
	union values
	{
		values() {}
		values(jsonList&& list) : list(list) {}
		values(jsonObject&& object) : object(object) {}
		values(const std::string& str) : str(str) {}
		values(int number) : number(number) {}
		values(bool boolean) : boolean(boolean) {}

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
