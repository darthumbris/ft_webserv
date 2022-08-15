#ifndef PARSE_HPP
#define PARSE_HPP

#include "Json.hpp"
#include <stdlib.h>

enum parseState
{
	CURLY_OPEN,
	CURLY_CLOSE,
	STRING,
	NUMBER,
	BOOLEAN,
	COLON,
	ARRAY_OPEN,
	ARRAY_CLOSE,
	COMMA,
	NULL_TYPE
};

struct Type
{
	std::string	value;
	parseState	type;
};

class Parse
{
	public:

		// Constructor
		Parse();

		// Destructor
		~Parse();

		// Getter
		Type	getToken();
		bool	hasMoreToken(std::istream& file) {return !file.eof();}

		// Member Functions
		void	skipWhiteSpaces(std::istream& file) {file >> std::ws;}
		std::string parseName(std::istream& file);
		Json*	parse(std::istream& file);

		// Exception
		class wrongToken: public std::exception {
		private:
			std::string _msg;
		public:
			wrongToken(const std::string& msg) : _msg(msg) {}
			virtual const char* what() const throw() {
				return _msg.c_str();
			}
		};
		
		

	private:

		// Member Functions
		Json	*parseOne(std::istream& file);
		Json	*parseObject(std::istream& file);
		Json	*parseArray(std::istream& file);
		Json	*parseString(std::istream& file);
		Json	*parseNumber(std::istream& file);
		Json	*parseBoolean(std::istream& file);
		Json	*parseNull(std::istream& file);

		// Getter
		Json::Token    getState(std::istream &file);

		std::stack<char> _state;
};

#endif