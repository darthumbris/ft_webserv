#include "CgiHandler.hpp"
#include "Utils.hpp"

// Constructors
CgiHandler::CgiHandler(RequestHandler &req) : _req(&req), _input_body(_req->getRequestBody()), _error(false)
{
	if (DEBUG_MODE)
		std::cout << "Cgi constructor called" << std::endl;
	std::cout << "input body: " << _input_body << std::endl;
}


// Destructor
CgiHandler::~CgiHandler()
{
}

// Setter
void	CgiHandler::setCgiPaths()
{
	//_folder ->req->getAbsPath of file to open
	//test/index.html
	//this would be test
	_folder = _req->getUrl().path.substr(0, _req->getUrl().path.find_last_of('/') + 1);
	//this would be the index.html
	//_file ->get file name to open
	_file = _req->getUrl().path.substr(_req->getUrl().path.find_last_of('/') + 1, _req->getUrl().path.length());
	//_file_name = ""/
	//
	_root = "/";
	/*
	*/
	
	// std::cout << "root: " << _root << std::endl;
	// std::cout << "folder: " << _folder << std::endl;
	// std::cout << "file: " << _file << std::endl;
	
//	_cur_dir = getCurDir();
//		_req->setCgiError();
//		_error = true;
//		return ;
//	}
	if (_cgi_path == "/usr/bin/python")
		_cgi_path = _cur_dir + _root + _folder + _file;
	if (DEBUG_MODE)
		std::cout << "cgipath: " << _cgi_path << std::endl;
}

//TODO have the redirect status be assigned in the script itself
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
	_env["SERVER_NAME="] = "ft_webserv"; // TODO get correct name from _req or from the headermap?
	_env["SERVER_PORT="] = std::to_string(_req->getPort());
	_env["SERVER_PROTOCOL="] = "HTTP/1.1";
	_env["SERVER_SOFTWARE="] = "test_server";
	_env["REDIRECT_STATUS="] = "200";
	_env["REQUEST_URI="] = _req->getUrl().path + _req->getUrl().querry;
	/*
	//TO DO fix this
	if (_req->getLocation(_folder)->getUploadPath() != "")
	{
		_env["UPLOAD_PATH="] = _req->getLocation(_folder)->getUploadPath();
		_env["ROOT_PATH="] = _cur_dir + "/" + _req->getLocation(_folder)->getRootPath();
	}
	*/

	if (DEBUG_MODE)
		for (auto it = _env.begin(); it != _env.end(); it++)
			std::cout << it->first << it->second << " len: " << it->second.length() << std::endl;
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
	_req->setCgiError();
	write(STDOUT_FILENO, "Status: 502\r\n\r\n", 15);
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
	if (_error)
		return "Status: 500\r\n\r\n";
	//reading in the input body to temp file
	write(_in_file_fd, _input_body.c_str(), _input_body.length());
	//Resetting to beginning of file
	lseek(_in_file_fd, 0, SEEK_SET);

	//forking for executing script
	pid = fork();
	if (pid == -1)
	{
		_req->setCgiError();
		return ("Status: 500\r\n\r\n");
	}
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
	if (DEBUG_MODE)
		std::cout << "finished executing " << std::endl;
	//TODO use the setContent-Type and setStatus from the request handler and substr the response and then return it
	return _output_body;
}

