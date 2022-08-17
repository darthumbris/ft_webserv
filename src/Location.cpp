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
	if (DEBUG_MODE)
	{
		std::cout << GREEN << "set the directory listing to: ";
		if (_auto_index)
			std::cout << "be enabled";
		else
			std::cout << "be disabled";
		std::cout << " for this location." <<  RESET_COLOR << std::endl;
	}
}

void Location::setUploadPath(const Json& json)
{
	_upload_path = json.values.str;
	if (DEBUG_MODE)
		std::cout << GREEN << "set the upload_path: " << _upload_path << " for this location" <<  RESET_COLOR << std::endl;
}

void Location::setReturnUrl(const Json& json)
{
	_return_url = json.values.str;
	if (DEBUG_MODE)
		std::cout << GREEN << "set return url: " << _return_url << " for this location." <<  RESET_COLOR << std::endl;
}

void Location::setRootFolder(const Json& json)
{
	_root = json.values.str;
	if (DEBUG_MODE)
		std::cout << GREEN << "set the root_folder: " << _root << " for this location" <<  RESET_COLOR << std::endl;
}

void Location::setGetMethod(const Json& json)
{
	(void)json;
	_allowed_methods.get = true;
	if (DEBUG_MODE)
		std::cout << GREEN << "Enabled the GET method for this location" <<  RESET_COLOR << std::endl;
}

void Location::setPostMethod(const Json& json)
{
	(void)json;
	_allowed_methods.post = true;
	if (DEBUG_MODE)
		std::cout << GREEN << "Enabled the POST method for this location" <<  RESET_COLOR << std::endl;
}

void Location::setDeleteMethod(const Json& json)
{
	(void)json;
	_allowed_methods.del = true;
	if (DEBUG_MODE)
		std::cout << GREEN << "Enabled the DELETE method for this location" <<  RESET_COLOR << std::endl;
}

void Location::setIndex(const Json& json)
{
	_index = json.values.str;
	if (DEBUG_MODE)
		std::cout << GREEN << "set the index_file: " << _index << " for this location" <<  RESET_COLOR << std::endl;
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
	if (DEBUG_MODE)
	{
		std::cout << GREEN  << "set the CGI file type: " << _cgi_file_type << " and the Cgi path: " << _cgi_path;
		std::cout << " for this location." <<  RESET_COLOR << std::endl;
	}
}


// Member Function
Location::Func	Location::which(const std::string& name, const Json &json)
{
	t_table	map[7] =
	{
			{"return", Json::STRING, &Location::setReturnUrl},
			{"auto_index", Json::BOOLEAN, &Location::setAutoIndex},
			{"index", Json::STRING, &Location::setIndex},
			{"root", Json::STRING, &Location::setRootFolder},
			{"allowed_method", Json::ARRAY,&Location::setAllowedMethod},
			{"CGI", Json::STRING,&Location::setCgi},
			{"upload", Json::STRING,&Location::setUploadPath},
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
	if (DEBUG_MODE)
		std::cout << GREEN << "\nAdding new location with dir: " << path << " to the server." << RESET_COLOR << std::endl;
	setLocation(json);
}
