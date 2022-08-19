#ifndef UTILS_HPP
# define UTILS_HPP

#include <sys/stat.h>
#include <mach-o/dyld.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

enum	path
{
	IS_DIR,
	IS_FILE,
	IS_OTHER
};

std::string 				getCurDir();
int	        				checkPath(const std::string &path, const std::string &root);
std::string					getContentType(const std::string &file_name);
std::string					HexToStr(const std::string &hex);
std::vector<std::string>	cpp_split(const std::string &line);
std::string 				trim(const std::string &s);

# endif
