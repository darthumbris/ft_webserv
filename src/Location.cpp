#include "Location.hpp"

// Constructors
Location::Location() : _auto_index(true)
{
	_allowed_methods.del = false;
	_allowed_methods.get = false;
	_allowed_methods.post = false;
}

Location::Location(const Location &copy)
{
	(void) copy;
}


// Destructor
Location::~Location()
{
}


// Operators
Location & Location::operator=(const Location &assign)
{
	(void) assign;
	return *this;
}

// Getters
bool		Location::getAutoIndex() const
{
	return _auto_index;
}

std::string	Location::getUploadPath() const
{
	return _upload_path;
}

std::string	Location::getReturnUrl() const
{
	return _return_url;
}

std::string	Location::getRootPath() const
{
	return _root;
}

bool		Location::getMethodGet() const
{
	return _allowed_methods.get;
}

bool		Location::getMethodPost() const
{
	return _allowed_methods.post;
}

bool		Location::getMethodDel() const
{
	return _allowed_methods.del;
}

// Setters
void	Location::setAutoIndex(bool auto_index)
{
	_auto_index = auto_index;
}

void	Location::setUploadPath(std::string upload_path)
{
	_upload_path = upload_path;
}

void	Location::setReturnUrl(std::string return_url)
{
	_return_url = return_url;
}

void	Location::setRootFolder(std::string root)
{
	_root = root;
}

void	Location::setGetMethod(bool get)
{
	_allowed_methods.get = get;
}

void	Location::setPostMethod(bool post)
{
	_allowed_methods.post = post;
}

void	Location::setDeleteMethod(bool del)
{
	_allowed_methods.del = del;
}
