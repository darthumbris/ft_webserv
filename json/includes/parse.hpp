#ifndef PARSE_HPP
#define PARSE_HPP

#include "json.hpp"
#include <stdlib.h>

class Json;

class Parse
{
private:
//	Json			json;

private:
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

private:
	struct Type
	{
		std::string	value;
		parseState	type;
	};

public:
	Type	getToken();
	std::stack<char> state;
	bool	hasMoreToken(std::istream& file) {
		return !file.eof();
	}
	void	skipWhiteSpaces(std::istream& file) {
		file >> std::ws;
	}
	std::string parseName(std::istream& file);

public:
	Parse();
	~Parse();

public:
	Json*	parse(std::istream& file);
private:
	Json	*parse_one(std::istream& file);
	Json	*parseObject(std::istream& file);
	Json	*parseArray(std::istream& file);
	Json	*parseString(std::istream& file);
	Json	*parseNumber(std::istream& file);
	Json	*parseBoolean(std::istream& file);
	Json	*parseNull(std::istream& file);

	Json::Token    getState(std::istream &file);

public:
	class wrongToken: public std::exception {
	private:
		std::string _msg;
	public:
		wrongToken(const std::string& msg) : _msg(msg) {}
		virtual const char* what() const throw() {
			return _msg.c_str();
		}
	};
};

#endif