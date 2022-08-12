#include "../includes/Location.hpp"

Location::Location() {

}

const std::string& Location::getRootPath() const {
	return _root;
}

bool Location::getAutoIndex() const {
	return _auto_index;
}

const std::string& Location::getUploadPath() const {
	return _upload_path;
}

const std::string& Location::getReturnUrl() const {
	return _return_url;
}

bool Location::getMethodGet() const {
	return _allowed_methods.get;
}

bool Location::getMethodPost() const {
	return _allowed_methods.post;
}

bool Location::getMethodDel() const {
	return _allowed_methods.del;
}

void Location::setAutoIndex(const Json& json) {
	_auto_index = json.values.boolean;
}

void Location::setUploadPath(const Json& json) {
	_upload_path = json.values.str;
}

void Location::setReturnUrl(const Json& json) {
	_return_url = json.values.str;
}

void Location::setRootFolder(const Json& json) {
	_root = json.values.str;
}

void Location::setGetMethod(const Json& json) {
	(void)json;
	_allowed_methods.get = true;
}

void Location::setPostMethod(const Json& json) {
	(void)json;
	_allowed_methods.post = true;
}

void Location::setDeleteMethod(const Json& json) {
	(void)json;
	_allowed_methods.del = true;
}

void Location::setIndex(const Json& json) {
	_index = json.values.str;
}

std::string Location::getEnumValue(Json::Token t) {
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

void Location::setAllowedMethod(const Json& json) {
	t_table	map[3] = {
			{"GET", Json::STRING, &Location::setGetMethod},
			{"POST", Json::STRING, &Location::setPostMethod},
			{"DELETE", Json::STRING, &Location::setDeleteMethod}
	};

	for (const Json *x: json.values.list) {
		int i = 0;
		for (const t_table& entry : map) {
			if (entry.type == x->type) {
				if (entry.key == x->values.str) {
					(this->*entry.map_values)(*x);
					break;
				}
			}
			i++;
			if (i == 3) {
				throw wrongType("expected <" + getEnumValue(x->type) + "> but got " + getEnumValue(entry.type));
			}
		}
	}
}

std::string Location::get_path(const std::string& path) {
	int i = 0;
	while (path[i] && !isspace(path[i])) {
		i++;
	}
	if (!path[i])
		return "";
	return path.substr(++i);
}

void Location::set_location(const Json& json) {
	for (const auto &x: json.values.object) {
		Location::Func f = which(x.first, *x.second);
		(*this.*f)(*x.second);
	}
}

void Location::get_location(const std::string& name, const Json& json) {
	if (json.type != Json::OBJECT) {
		throw wrongType("expected <" + getEnumValue(Json::OBJECT) + "> after a location");
	}
	std::string path = get_path(name);
	if (path.empty()) {
		wrongType("path cannot be empty");
	}
	_upload_path = path;
	set_location(json);
}

Location::Func	Location::which(const std::string& name, const Json &json) {
	t_table	map[5] = {
			{"return", Json::STRING, &Location::setReturnUrl},
			{"auto_index", Json::BOOLEAN, &Location::setAutoIndex},
			{"index", Json::STRING, &Location::setIndex},
			{"root", Json::STRING, &Location::setRootFolder},
			{"allowed_method", Json::ARRAY,&Location::setAllowedMethod},
	};

	for (const t_table& entry : map) {
		if (entry.type == json.type) {
			if (entry.key == name) {
				return (entry.map_values);
			}
		}
	}
	throw wrongType("expected <testing>");
}

Location::~Location() {

}

//TODO the location need always a root, client_body_size and
