#ifndef UTILS_HPP
# define UTILS_HPP

#include <sys/stat.h>
#include <mach-o/dyld.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

# ifndef DEBUG_MODE
#  define DEBUG_MODE	0
# endif

enum	path
{
	IS_DIR,
	IS_FILE,
	IS_OTHER
};

std::string getCurDir();
int	        checkPath(std::string path, std::string root);
std::string	getContentType(std::string file_name);

# endif
