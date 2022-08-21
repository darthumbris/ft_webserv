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
std::vector<std::string>	cpp_split(const std::string &line, char delimiter);
std::string 				trim(const std::string &s, const std::string &to_trim);
std::string					stripExesSlashes(const std::string &uri);
std::size_t					LengthOfMatch(const std::string &str1, const std::string &str2);

# endif
