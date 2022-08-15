#include "CgiHandler.hpp"
#include <mach-o/dyld.h>

//TODO fix this, now doesnt properly give a return that makes sense

static std::string getCurDir()
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


// Constructors
CgiHandler::CgiHandler(RequestHandler &req) : _req(&req)
{
	_cgi_path = "/Users/shoogenb/.brew/bin/php-cgi"; //TODO load this from config

	std::string	request = _req->getCompleteRequest();
	std::size_t	body_start = request.find("\r\n\r\n");
	_input_body = request.substr(body_start + 4);
	std::cout << "input_body: " << _input_body << std::endl;
	std::cout << "length: " << _input_body.length() << std::endl;
}


// Destructor
CgiHandler::~CgiHandler()
{
}

// Setter
void	CgiHandler::setCgiPaths()
{
	_folder = _req->getUrl().path.substr(0, _req->getUrl().path.find_last_of('/'));
	_file = _req->getUrl().path.substr(_req->getUrl().path.find_last_of('/') + 1, _req->getUrl().path.length());
	if (_req->getLocation(_folder))
		_root = _req->getLocation(_folder)->getRootPath();
	else
		_root = "";
	
	_root.pop_back();
	_folder.push_back('/');
	_cur_dir = getCurDir();
}

void	CgiHandler::setEnvValues()
{
	setCgiPaths();

	_env["AUTH_TYPE="] = _req->getHeaderMap()["Authorization"];
	_env["CONTENT_LENGTH="] = _req->getHeaderMap()["Content-Length"];
	_env["CONTENT_TYPE="] = _req->getHeaderMap()["Content-Type"];
	_env["GATEWAY_INTERFACE="] = "CGI/1.1";
	_env["PATH_INFO="] = _file;
	_env["PATH_TRANSLATED="] = _cur_dir + _root + _folder + _file;
	_env["QUERY_STRING="] = _req->getUrl().querry;
	_env["REMOTE_ADDR="] = _req->getClientIp();
	_env["REMOTEaddr="] = _req->getClientIp();
	_env["REMOTE_IDENT="] = _req->getHeaderMap()["Authorization"];
	_env["REMOTE_USER="] = _req->getHeaderMap()["Authorization"];
	_env["REQUEST_METHOD="] = _req->getRequestMethod();
	_env["SCRIPT_NAME="] = _cgi_path;
	_env["SERVER_NAME="] = "test_server"; // TODO get correct name from _req
	_env["SERVER_PORT="] = std::to_string(_req->getPort());
	_env["SERVER_PROTOCOL="] = "HTTP/1.1";
	_env["SERVER_SOFTWARE="] = "test_server";
	_env["REDIRECT_STATUS="] = "200";
	_env["REQUEST_URI="] = _req->getUrl().path + _req->getUrl().querry;

	// std::cout << "current dir:" << _cur_dir << std::endl;
	// std::cout << "path_info:" << _env["PATH_INFO="] << std::endl;
	// std::cout << "path_translated:" << _env["PATH_TRANSLATED="] << std::endl;
	// std::cout << "content_length: " << _env["CONTENT_LENGTH="] << std::endl;
	// std::cout << "REQUEST_METHOD: " << _env["REQUEST_METHOD="] << std::endl;
	// for (auto it = _env.begin(); it != _env.end(); it++)
	// 	std::cout << it->first << it->second << " len: " << it->second.length() << std::endl;


	// std::cout << "\n-----end of cgihandler env setter--------" << std::endl;
}

// this is for testing need to make a proper version for this
char**	CgiHandler::makeEnvArray()
{
	char **envp;
	int	i = 0;
	
	//maybe a try catch for this?
	envp = new char* [_env.size() + 1];

	t_strmap::iterator it = _env.begin();
	for (; it != _env.end(); it++, i++)
	{
		envp[i] = new char[it->first.size() + it->second.size() + 1];
		strcpy(envp[i], (it->first + it->second).c_str());
	}
	envp[i] = NULL;
	return (envp);
}

void	CgiHandler::executeScript(char **envp)
{
	dup2(_in_file_fd, STDIN_FILENO);
	dup2(_out_file_fd, STDOUT_FILENO);
	execve(_cgi_path.c_str(), NULL, envp);
	write(STDOUT_FILENO, "Status: 500\r\n", 15);
}

void	CgiHandler::readScriptOutput(pid_t pid)
{	
	char	buffer[65536] = {0};

	waitpid(pid, NULL, 0);
	lseek(_out_file_fd, 0, SEEK_SET);

	while (read(_out_file_fd, buffer, 65536) > 0)
	{
		_output_body += buffer;
		memset(buffer, 0, 65536);
	}
}

void	CgiHandler::setFileDescriptors()
{
	_temp_in_fd = dup(STDIN_FILENO);
	_temp_out_fd = dup(STDOUT_FILENO);
	_tmp_in_file = tmpfile();
	_tmp_out_file = tmpfile();
	_in_file_fd = fileno(_tmp_in_file);
	_out_file_fd = fileno(_tmp_out_file);
}

void	CgiHandler::closeFileDescriptors()
{
	dup2(_temp_in_fd, STDIN_FILENO);
	dup2(_temp_out_fd, STDOUT_FILENO);
	fclose(_tmp_in_file);
	fclose(_tmp_out_file);
	close(_in_file_fd);
	close(_out_file_fd);
	close(_temp_in_fd);
	close(_temp_out_fd);
}

std::string	CgiHandler::execute()
{
	char		**envp;
	pid_t		pid;

	setEnvValues();
	setFileDescriptors();

	envp = makeEnvArray();

	//reading in the input body to temp file
	write(_in_file_fd, _input_body.c_str(), _input_body.length());
	//Resetting to beginning of file
	lseek(_in_file_fd, 0, SEEK_SET);

	//forking for executing script
	pid = fork();
	if (pid == -1)
		return ("Status: 500\r\n");
	else if (pid == 0)
		executeScript(envp);
	else
		readScriptOutput(pid);

	closeFileDescriptors();

	//freeing envp
	for (std::size_t i = 0; envp[i]; i++)
		delete[] envp[i];
	delete envp;

	if (pid == 0)
		exit(0);
	std::cout << "finished executing " << std::endl;

	//TODO check if this works always or needs to be done differently
	std::size_t	start_content_type = _output_body.find("Content-type");
	std::size_t	start_body = _output_body.find('\n', start_content_type);
	_output_body = _output_body.substr(start_body, std::string::npos);
	return _output_body;
}
