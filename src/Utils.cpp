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

//https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
std::map<std::string, std::string>	getTypeMap()
{
	std::map<std::string, std::string>	type_map;

	type_map["aac"] = "audio/aac";
	type_map["abw"] = "application/x-abiword";
	type_map["arc"] = "application/x-freearc";
	type_map["avif"] = "image/avif";
	type_map["avi"] = "video/x-msvideo";
	type_map["azw"] = "application/vnd.amazon.ebook";
	type_map["bin"] = "application/octet-stream";
	type_map["bmp"] = "image/bmp";
	type_map["bz"] = "application/x-bzip";
	type_map["bz2"] = "application/x-bzip2";
	type_map["cda"] = "application/x-cdf";
	type_map["csh"] = "application/x-csh";
	type_map["css"] = "text/css";
	type_map["csv"] = "text/csv";
	type_map["doc"] = "application/msword";
	type_map["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	type_map["eot"] = "application/vnd.ms-fontobject";
	type_map["epub"] = "application/epub+zip";
	type_map["gz"] = "application/gzip";
	type_map["gif"] = "image/gif";
	type_map["html"] = "text/html";
	type_map["htm"] = "text/html";
	type_map["ico"] = "image/vnd.microsoft.icon";
	type_map["ics"] = "text/calendar";
	type_map["jar"] = "application/java-archive";
	type_map["jpeg"] = "image/jpeg";
	type_map["jpg"] = "image/jpeg";
	type_map["js"] = "text/javascript";
	type_map["json"] = "application/json";
	type_map["jsonld"] = "application/ld+json";
	type_map["mid"] = "audio/midi";
	type_map["midi"] = "audio/midi";
	type_map["mjs"] = "text/javascript";
	type_map["mp3"] = "audio/mpeg";
	type_map["mp4"] = "video/mp4";
	type_map["mpeg"] = "video/mpeg";
	type_map["mpkg"] = "application/vnd.apple.installer+xml";
	type_map["odp"] = "application/vnd.oasis.opendocument.presentation";
	type_map["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	type_map["odt"] = "application/vnd.oasis.opendocument.text";
	type_map["oga"] = "audio/ogg";
	type_map["ogv"] = "video/ogg";
	type_map["ogx"] = "application/ogg";
	type_map["opus"] = "audio/opus";
	type_map["otf"] = "font/otf";
	type_map["png"] = "image/png";
	type_map["pdf"] = "application/pdf";
	type_map["php"] = "application/x-httpd-php";
	type_map["ppt"] = "application/vnd.ms-powerpoint";
	type_map["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	type_map["rar"] = "application/vnd.rar";
	type_map["rtf"] = "application/rtf";
	type_map["sh"] = "application/x-sh";
	type_map["svg"] = "image/svg+xml";
	type_map["swf"] = "application/x-shockwave-flash";
	type_map["tar"] = "application/x-tar";
	type_map["tif"] = "image/tiff";
	type_map["tiff"] = "image/tiff";
	type_map["ts"] = "video/mp2t";
	type_map["ttf"] = "font/ttf";
	type_map["txt"] = "text/plain";
	type_map["vsd"] = "application/vnd.visio";
	type_map["wav"] = "audio/wav";
	type_map["weba"] = "audio/webm";
	type_map["webm"] = "video/webm";
	type_map["webp"] = "image/webp";
	type_map["woff"] = "font/woff";
	type_map["woff2"] = "font/woff2";
	type_map["xhtml"] = "application/xhtml+xml";
	type_map["xls"] = "application/vnd.ms-excel";
	type_map["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	type_map["xml"] = "application/xml";
	type_map["xul"] = "application/vnd.mozilla.xul+xml";
	type_map["zip"] = "application/zip";
	type_map["3gp"] = "video/3gpp";
	type_map["3g2"] = "video/3gpp2";
	type_map["7z"] = "application/x-7z-compressed";
	return type_map;
}

std::string	getContentType(std::string file_name)
{
	if (DEBUG_MODE)
		std::cout << "Looking for content-type of file: " << file_name << std::endl;
	static const std::map<std::string, std::string>	type_map = getTypeMap();
	std::size_t type_pos = file_name.find_last_of('.');
	if (type_pos != std::string::npos)
	{
		std::string	file_type = file_name.substr(type_pos + 1);
		if (type_map.find(file_type) != type_map.end())
			return type_map.at(file_type);
	}
	return "application/octet-stream";
}