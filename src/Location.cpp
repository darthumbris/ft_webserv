#include "Location.hpp"

// Constructors
Location::Location()
{
	_auto_index = true;
	_upload_path = "/upload";
	_return_url = "google.com";
	_allowed_methods.del = true;
	_allowed_methods.get = true;
	_allowed_methods.post = true;
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

