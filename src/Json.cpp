#include "Json.hpp"
#include "Config.hpp"

// Constructor
Json::Json() {}

// Destructor
Json::~Json()
{
	switch (type)
	{
		case STRING:
			values.str.~basic_string();
			break;
		case ARRAY:
			for (jsonList::iterator x = values.list.begin(); x != values.list.end(); x++)
			{
				delete *x;
			}
			values.list.~vector();
			break;
		case OBJECT:
			for (jsonObject::iterator x = values.object.begin(); x != values.object.end(); x++)
			{
				delete x->second;
			}
			values.object.~map();
			break;
		default:
			break;
	}
}

// Member Function
void Json::print() const
{
	switch (type)
	{
		case STRING:
			std::cout << "String ";
			std::cout << values.str;
			break;
		case NUMBER:
			std::cout << "Number ";
			std::cout << values.number;
			break;
		case BOOLEAN:
			std::cout << "boolean ";
			std::cout << std::boolalpha << values.boolean;
			break;
		case ARRAY:
			std::cout << "Array [ ";
			for (const Json *x: values.list) {
				x->print();
				std::cout << ", ";
			}
			std::cout << " ]";
			break;
		case OBJECT:
			std::cout << "Object { ";
			for (const auto &x: values.object) {
				std::cout << x.first << ": ";
				x.second->print();
			}
			std::cout << " }";
			break;
		case NULL_TYPE:
		case ERROR:
			std::cout << "null";
			break;
	}
}

// Operators
std::ostream& operator<<(std::ostream& out, const Json::Token& t)
{
	switch (t)
	{
		case Json::STRING:
			out << "STRING";
			break;
		case Json::NUMBER:
			out << "NUMBER";
			break;
		case Json::BOOLEAN:
			out << "BOOLEAN";
			break;
		case Json::ARRAY:
			out << "ARRAY";
			break;
		case Json::OBJECT:
			out << "OBJECT";
			break;
		case Json::NULL_TYPE:
		case Json::ERROR:
			out << "NULL_TYPE";
			break;
	}
	return out;
}
