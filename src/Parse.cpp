#include "Parse.hpp"

// Constructor
Parse::Parse() {}

// Destructor
Parse::~Parse() {}

// Getters
Json::Token Parse::getState(std::istream& file)
{
    skipWhiteSpaces(file);
    char type = file.peek();
    switch (type)
	{
		case '"':
			return Json::Token::STRING;
		case 't':
		case 'f':
			return Json::Token::BOOLEAN;
		case '[':
			return Json::Token::ARRAY;
		case '{':
			return Json::Token::OBJECT;
		case 'n':
			return Json::Token::NULL_TYPE;
		default:
			if (type == '-' || isdigit(type))
				return Json::Token::NUMBER;
			else
				return Json::Token::ERROR;
    }
}

std::string Parse::parseName(std::istream &file)
{
	std::string str;
	char c = file.get();
	if (c != '"')
		return "";
	auto done  =  [] (char c) { return c != '"'; };
	while (done(file.peek()))
		str += file.get();
	file.get();
	skipWhiteSpaces(file);
	if (file.get() != ':')
		return "";
	skipWhiteSpaces(file);
	return str;
}

Json *Parse::parseObject(std::istream& file)
{
	Json *node = new Json(jsonObject());
	node->type = Json::Token::OBJECT;
	std::string name;
	char c;
	while (file.good())
	{
		c = file.get();
		skipWhiteSpaces(file);
		if (c == '{')
		{
			if (file.peek() == '}')
			{
				file.get();
				return node;
			}
			_state.push(c);
			name = parseName(file);
			if (name.empty())
			{
				delete node;
				return NULL;
			}
			Json* next = parseOne(file);
			node->values.object.insert(std::make_pair(name, next));
		} 
		else if (_state.top() == '{' && c == '}') 
		{
			_state.pop();
			return node;
		}
		else if (c == ',')
		{
			name = parseName(file);
			if (name.empty())
			{
				delete node;
				return NULL;
			}
			Json* next = parseOne(file);
			node->values.object.insert(std::make_pair(name, next));
		}
		else
		{
			delete node;
			return NULL;
		}
		skipWhiteSpaces(file);
	}
	throw wrongToken("syntax error");
}

Json *Parse::parseArray(std::istream& file)
{
	Json *node = new Json(jsonList());
	node->type = Json::Token::ARRAY;
	char c;

	while (file.good())
	{
		c = file.get();
		skipWhiteSpaces(file);
		if (c == '[')
		{
			if (file.peek() == ']')
			{
				file.get();
				return node;
			}
			_state.push(c);
			node->values.list.push_back(parseOne(file));
		}
		else if (_state.top() == '[' && c == ']')
		{
			_state.pop();
			return node;
		}
		else if (c == ',')
		{
			Json* next = parseOne(file);
			node->values.list.push_back(next);
		}
		else
		{
			delete node;
			return NULL;
		}
		skipWhiteSpaces(file);
	}
	delete node;
	return NULL;
}

Json *Parse::parseString(std::istream& file)
{
	Json *node = new Json(std::string());
	std::string str;
	file.ignore();
	while (file.good())
	{
		if (file.peek() != '"')
			str += file.get();
		else
			break;
	}
	if (file.get() != '"')
	{
		delete node;
		return NULL;
	}
	node->values.str = str;
	node->type = Json::STRING;
	return node;
}

Json *Parse::parseNumber(std::istream& file)
{
	Json *node = new Json(int());
	std::string str;
	if (file.peek() == '-')
		str += file.get();
	while (isdigit(file.peek()))
		str += file.get();
	node->values.number = std::stoi(str);
	node->type = Json::NUMBER;
	skipWhiteSpaces(file);
	return node;
}

Json *Parse::parseBoolean(std::istream& file)
{
	Json *node = new Json(bool());
	char correct[5];
	char wrong[6];
	if (file.peek() == 't')
	{
		file.get(correct, sizeof(correct));
		if (std::memcmp("true", correct, 4) != 0)
		{
			delete node;
			return NULL;
		}
		node->values.boolean = true;
	}
	else
	{
		file.get(wrong, sizeof(wrong));
		if (std::memcmp("false", wrong, 5) != 0)
		{
			delete node;
			return NULL;
		}
		node->values.boolean = false;
	}
	node->type = Json::BOOLEAN;
	skipWhiteSpaces(file);
	return node;
}

Json *Parse::parseNull(std::istream& file)
{
	Json *node = new Json;
	char str[5];
	file.get(str, sizeof(str));
	if (std::memcmp("null", str, 4) != 0)
	{
		delete node;
		return NULL;
	}
	node->type = Json::NULL_TYPE;
	skipWhiteSpaces(file);
	return node;
}

Json *Parse::parse(std::istream& file)
{
	Json *node = nullptr;
    if (hasMoreToken(file))
		node = parseOne(file);
	else
		node = new Json();
	if (!_state.empty())
	{
		delete node;
		throw wrongToken("invalid json filesssss");
	}
	return node;
}

Json *Parse::parseOne(std::istream& file)
{
	Json::Token c = getState(file);

	switch (c)
	{
		case Json::NUMBER:
			return parseNumber(file);
		case Json::BOOLEAN:
			return parseBoolean(file);
		case Json::ARRAY:
			return parseArray(file);
		case Json::OBJECT:
			return parseObject(file);
		case Json::NULL_TYPE:
			return parseNull(file);
		case Json::STRING:
			return parseString(file);
		default:
			return NULL;
	}
}
