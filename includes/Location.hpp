#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>

typedef struct accepted_methods
{
	bool	get;
	bool	post;
	bool	del;
}	t_methods;


//TODO add setter and getter functions
class Location
{
	public:
		// Constructors
		Location();
		Location(const Location &copy);
		
		// Destructor
		~Location();

		// Getters
		bool		getAutoIndex() const;
		std::string	getUploadPath() const;
		std::string	getReturnUrl() const;
		std::string	getRootPath() const;
		bool		getMethodGet() const;
		bool		getMethodPost() const;
		bool		getMethodDel() const;

		// Setters
		void	setAutoIndex(bool auto_index);
		void	setUploadPath(std::string upload_path);
		void	setReturnUrl(std::string return_url);
		void	setRootFolder(std::string root);
		void	setGetMethod(bool get);
		void	setPostMethod(bool post);
		void	setDeleteMethod(bool del);

		// Operators
		Location & operator=(const Location &assign);
		
	private:
		bool						_auto_index;
		std::string					_upload_path;
		std::string					_return_url;
		std::string					_root;
		std::vector<std::string>	_index_file;
		t_methods					_allowed_methods;
};

#endif
