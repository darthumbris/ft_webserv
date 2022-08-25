#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Json.hpp"

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
		Location(const Location& assign);		

		// Destructor
		~Location();

		// Getters
		bool				getAutoIndex() const;
		const std::string&	getUploadPath() const;
		const std::string&	getReturnUrl() const;
		const std::string&	getRootPath() const;
		bool				getMethodGet() const;
		bool				getMethodPost() const;
		bool				getMethodDel() const;
		const std::string&	getIndex() const;
		std::string 		getPath(const std::string& path);
		std::string			getEnumValue(Json::Token type);
		std::string			getCgiPath() const;

		typedef void (Location::* Func )(const Json&);

		//Table to check which function to jump in
		typedef struct s_table
		{
			std::string	key;
			Json::Token	type;
			Func		map_values;
		}	t_table;

		// Setters
		void	setCgi(const Json& json);
		void 	setLocation(const Json& json);
		void	setAutoIndex(const Json& json);
		void	setUploadPath(const Json& json);
		void	setIndex(const Json& json);
		void	setReturnUrl(const Json& json);
		void	setRootFolder(const Json& json);
		void	setGetMethod(const Json& json);
		void	setPostMethod(const Json& json);
		void	setDeleteMethod(const Json& json);
		void	setAllowedMethod(const Json& json);

		// Operators
		Location & operator=(const Location& assign);

		
		// Member Function
		Func	which(const std::string& name, const Json& json);
		void	ParseLocation(const std::string& name, const Json& json);

		
		// Exception
		class wrongType: public std::exception 
		{
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
		std::string					_cgi_path;
		std::string					_cgi_file_type;
		std::string 				_index;
		std::string					_upload_path;
		std::string					_return_url;
		std::string					_return_code;
		std::string					_root;
		std::vector<std::string>	_index_file;
		t_methods					_allowed_methods;
};

#endif
