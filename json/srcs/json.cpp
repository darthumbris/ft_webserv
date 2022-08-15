#include "../includes/json.hpp"
#include "../includes/config.hpp"

Json::Json() {}

void Json::print() const {
	switch (type) {
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
			std::cout << "null";
			break;
	}
}

std::ostream& operator<<(std::ostream& out, const Json::Token& t) {
	switch (t) {
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
			out << "NULL_TYPE";
			break;
	}
	return out;
}

Json::~Json() {}