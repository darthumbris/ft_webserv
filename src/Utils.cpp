#include "Utils.hpp"

std::string getCurDir()
{
	unsigned int bufferSize = 512;
	std::vector<char> buffer(bufferSize + 1);

	if(_NSGetExecutablePath(&buffer[0], &bufferSize))
	{
		buffer.resize(bufferSize);
		_NSGetExecutablePath(&buffer[0], &bufferSize);
	}
	std::string cur_dir = &buffer[0];
	cur_dir.resize(cur_dir.length() - 10);
	return cur_dir;
}

int	checkPath(std::string path, std::string root)
{
	struct stat s;

	std::string	total_path = getCurDir() + "/" + root + path;
	if (stat(total_path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			std::cout << "is a directory" << std::endl;
			return IS_DIR;
		}
		else if (s.st_mode & S_IFREG)
		{
			std::cout << "is a file" << std::endl;
			return IS_FILE;
		}
		else
		{
			std::cout << "is something else" << std::endl;
			return IS_OTHER;
		}
	}
	std::cout << "Error: " << total_path << " is not a file or directory" << std::endl;
	return IS_OTHER;
}
