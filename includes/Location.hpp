#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>

struct accepted_methods
{
	bool	get;
	bool	post;
	bool	del;
};


class Location
{
	public:
		// Constructors
		Location();
		Location(const Location &copy);
		
		// Destructor
		~Location();
		
		// Operators
		Location & operator=(const Location &assign);
		
	private:
		bool						_auto_index;
		std::string					_upload_path;
		std::string					_return_url;
		std::vector<std::string>	_index_file;
		struct accepted_methods		_allowed_methods;
};

#endif
