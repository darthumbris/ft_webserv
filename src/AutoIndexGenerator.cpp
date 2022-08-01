#include "AutoIndexGenerator.hpp"

// Constructors
AutoIndexGenerator::AutoIndexGenerator(const std::string root, const std::string path) : _path(path)
{
	DIR	*dir = opendir(_path.c_str());

	//Making the start of directory listing.
	this->_directoryIndex += "<html>\n <head>\n <title>Index of /";
	this->_directoryIndex += _path;
	this->_directoryIndex += "</title>\n </head>\n <body>";
	this->_directoryIndex += "<h1>Index of /";
	this->_directoryIndex += _path;
	this->_directoryIndex += "</h1>";
	this->_directoryIndex += "<ul>\n";

	if (!dir)
	{
		std::cout << "Error opening directory" << std::endl;
		return ;
	}
	//Going through all files in the directory to
	struct dirent	*currentDirEntry;
	while ((currentDirEntry = readdir(dir)))
	{
		if (!isCurrentOrParentDirectory(currentDirEntry->d_name))
		{
			this->_directoryIndex += "<li><a href=\"";
			this->_directoryIndex += root;
			if (root[root.length() - 1] == '/')
				this->_directoryIndex += "";
			else
				this->_directoryIndex += "/";
			this->_directoryIndex += std::string(currentDirEntry->d_name) + "\">";
			this->_directoryIndex += currentDirEntry->d_name;
			this->_directoryIndex += "</a></li>\n";
		}
	}

	//Closing the body and html
	this->_directoryIndex += "</ul></body>\n</html>";
	closedir(dir);
}

AutoIndexGenerator::AutoIndexGenerator(const AutoIndexGenerator &copy)
{
	(void) copy;
}


// Destructor
AutoIndexGenerator::~AutoIndexGenerator()
{
}


// Operators
AutoIndexGenerator & AutoIndexGenerator::operator=(const AutoIndexGenerator &assign)
{
	(void) assign;
	return *this;
}

// Getters
std::string	AutoIndexGenerator::getDirectoryIndex() const
{
	return this->_directoryIndex;
}

bool	AutoIndexGenerator::isCurrentOrParentDirectory(std::string name) const
{
	return (strcmp(name.c_str(), ".") == 0 || strcmp(name.c_str(), "..") == 0);
}