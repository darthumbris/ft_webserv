#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>
#include "../json/includes/json.hpp"

struct accepted_methods
{
	bool	get;
	bool	post;
	bool	del;
};

using t_methods = struct accepted_methods;

class Location
{
public:
	// Constructors
	Location();
//	Location(const Location &copy);

	Location *_values;

	// Destructor
	~Location();

	// Getters
	bool		getAutoIndex() const;
	const std::string&	getUploadPath() const;
	const std::string&	getReturnUrl() const;
	const std::string&	getRootPath() const;
	bool		getMethodGet() const;
	bool		getMethodPost() const;
	bool		getMethodDel() const;

	typedef void (Location::* Func )(const Json&);

	//Table to check which function to jump in
	typedef struct s_table {
		std::string	key;
		Json::Token	type;
		Func		map_values;
	}	t_table;

	// Setters
	void	setAutoIndex(const Json& json);
	void	setUploadPath(const Json& json);
	void	setIndex(const Json& json);
	void	setReturnUrl(const Json& json);
	void	setRootFolder(const Json& json);
	void	setGetMethod(const Json& json);
	void	setPostMethod(const Json& json);
	void	setDeleteMethod(const Json& json);
	void	setAllowedMethod(const Json& json);

	std::string get_path(const std::string& path);

	// Operators
	Location & operator=(const Location& assign);

	void	get_location(const std::string& name, const Json& json);
	void 	set_location(const Json& json);
	Func	which(const std::string& name, const Json& json);

	std::string	getEnumValue(Json::Token type);

	class wrongType: public std::exception {
	private:
		std::string _msg;
	public:
		wrongType(const std::string& msg) : _msg(msg) {}
		virtual const char* what() const throw() {
			return _msg.c_str();
		}
	};

private:
	bool						_auto_index;
	std::string 				_index;
	std::string					_upload_path;
	std::string					_return_url;
	std::string					_root;
	std::vector<std::string>	_index_file;
	t_methods					_allowed_methods;
};

#endif