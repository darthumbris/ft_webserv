#ifndef AUTOINDEXGENERATOR_HPP
# define AUTOINDEXGENERATOR_HPP

# include <iostream>
# include <string>
# include <dirent.h>
# include <sys/types.h>

class AutoIndexGenerator
{
	public:
		// Constructors
		AutoIndexGenerator(const std::string &root, const std::string &path);
		AutoIndexGenerator(const AutoIndexGenerator &copy);
		
		// Destructor
		~AutoIndexGenerator();
		
		// Operators
		AutoIndexGenerator & operator=(const AutoIndexGenerator &assign);

		// Getters
		std::string	getDirectoryIndex() const;
		bool		isCurrentDirectory(std::string name) const;
		
	private:

		std::string	_directoryIndex;
		std::string	_path;
		
};

#endif
