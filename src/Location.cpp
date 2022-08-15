#include "../includes/Location.hpp"

// Constructor
Location::Location()
{
}

// Destructor
Location::~Location()
{
}

// Getters
const std::string& Location::getRootPath() const
{
	return _root;
}

bool Location::getAutoIndex() const
{
	return _auto_index;
}

const std::string& Location::getUploadPath() const
{
	return _upload_path;
}

const std::string& Location::getReturnUrl() const
{
	return _return_url;
}

bool Location::getMethodGet() const
{
	return _allowed_methods.get;
}

bool Location::getMethodPost() const
{
	return _allowed_methods.post;
}

bool Location::getMethodDel() const
{
	return _allowed_methods.del;
}

std::string Location::getEnumValue(Json::Token t)
{
	switch (t) {
		case Json::STRING:
			return "STRING";
		case Json::NUMBER:
			return "NUMBER";
		case Json::BOOLEAN:
			return "BOOLEAN";
		case Json::ARRAY:
			return "ARRAY";
		case Json::OBJECT:
			return "OBJECT";
		default:
			return "NULL_TYPE";
	}
}

std::string Location::getPath(const std::string& path)
{
	int i = 0;
	while (path[i] && !isspace(path[i]))
		i++;
	if (!path[i])
		return "";
	return path.substr(++i);
}

const std::string&	Location::getIndex() const
{
	return this->_index;
}

std::string			Location::getCgiPath() const
{
	return this->_cgi_path;
}

// Setters
void Location::setAutoIndex(const Json& json)
{
	_auto_index = json.values.boolean;
}

void Location::setUploadPath(const Json& json)
{
	_upload_path = json.values.str;
}

void Location::setReturnUrl(const Json& json)
{
	_return_url = json.values.str;
}

void Location::setRootFolder(const Json& json)
{
	_root = json.values.str;
}

void Location::setGetMethod(const Json& json)
{
	(void)json;
	_allowed_methods.get = true;
}

void Location::setPostMethod(const Json& json)
{
	(void)json;
	_allowed_methods.post = true;
}

void Location::setDeleteMethod(const Json& json)
{
	(void)json;
	_allowed_methods.del = true;
}

void Location::setIndex(const Json& json)
{
	_index = json.values.str;
}

void Location::setAllowedMethod(const Json& json)
{
	t_table	map[3] =
	{
			{"GET", Json::STRING, &Location::setGetMethod},
			{"POST", Json::STRING, &Location::setPostMethod},
			{"DELETE", Json::STRING, &Location::setDeleteMethod}
	};

	for (const Json *x: json.values.list)
	{
		int i = 0;
		for (const t_table& entry : map)
		{
			if (entry.type == x->type)
			{
				if (entry.key == x->values.str)
				{
					(this->*entry.map_values)(*x);
					break;
				}
			}
			i++;
			if (i == 3)
				throw wrongType("expected <" + getEnumValue(x->type) + "> but got " + getEnumValue(entry.type));
		}
	}
}

void Location::setLocation(const Json& json)
{
	for (const auto &x: json.values.object)
	{
		Location::Func f = which(x.first, *x.second);
		(*this.*f)(*x.second);
	}
}

void	Location::setCgi(const Json& json)
{
	_cgi_file_type = json.values.str.substr(0, json.values.str.find_first_of(" "));
	_cgi_path = json.values.str.substr(json.values.str.find_first_of(" ") + 1, std::string::npos);
	// std::cout << "CGI file type: " << _cgi_file_type << "\nCgi path: " << _cgi_path << std::endl;
}


// Member Function
Location::Func	Location::which(const std::string& name, const Json &json)
{
	t_table	map[6] =
	{
			{"return", Json::STRING, &Location::setReturnUrl},
			{"auto_index", Json::BOOLEAN, &Location::setAutoIndex},
			{"index", Json::STRING, &Location::setIndex},
			{"root", Json::STRING, &Location::setRootFolder},
			{"allowed_method", Json::ARRAY,&Location::setAllowedMethod},
			{"CGI", Json::STRING,&Location::setCgi},
	};

	for (const t_table& entry : map)
	{
		if (entry.type == json.type)
		{
			if (entry.key == name)
				return (entry.map_values);
		}
	}
	throw wrongType("expected <testing>");
}

void Location::ParseLocation(const std::string& name, const Json& json)
{
	if (json.type != Json::OBJECT)
		throw wrongType("expected <" + getEnumValue(Json::OBJECT) + "> after a location");
	std::string path = getPath(name);
	if (path.empty())
		wrongType("path cannot be empty");
	_upload_path = path;
	setLocation(json);
}
