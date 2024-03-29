#include "CgiHandler.hpp"
#include "Utils.hpp"

// Constructors
CgiHandler::CgiHandler(RequestHandler &req) : _req(&req), _input_body(_req->getRequestBody()), _error(false)
{
	if (DEBUG_MODE)
		std::cout << "Cgi constructor called" << std::endl;
}


// Destructor
CgiHandler::~CgiHandler()
{
}

// Setter
void	CgiHandler::setCgiPaths()
{
	Location	loc = _req->getMatchingLocation();

	_file = _req->getFileName();
	_cgi_path = loc.getCgiPath();
	_cur_dir = getCurDir();
	if (_cgi_path == "/usr/bin/python")
		_cgi_path = _file;
	if (DEBUG_MODE)
	{
		std::cout << "url path: " << _req->getUrl().path << std::endl;
		std::cout << "url querry: " << _req->getUrl().querry << std::endl;
		std::cout << "cgipath: " << _cgi_path << std::endl;
		std::cout << "file: " << _file << std::endl;
	}
}

void	CgiHandler::setEnvValues()
{
	setCgiPaths();

	_env["AUTH_TYPE="] = _req->getHeaderMap()["Authorization"];
	_env["CONTENT_LENGTH="] = _req->getHeaderMap()["Content-Length"];
	_env["CONTENT_TYPE="] = _req->getHeaderMap()["Content-Type"];
	_env["GATEWAY_INTERFACE="] = "CGI/1.1";
	_env["PATH_INFO="] = _file;
	_env["PATH_TRANSLATED="] = _file;
	_env["QUERY_STRING="] = _req->getUrl().querry;
	_env["REMOTE_ADDR="] = _req->getClientIp();
	_env["REMOTEaddr="] = _req->getClientIp();
	_env["REMOTE_IDENT="] = _req->getHeaderMap()["Authorization"];
	_env["REMOTE_USER="] = _req->getHeaderMap()["Authorization"];
	_env["REQUEST_METHOD="] = _req->getRequestMethod();
	_env["SCRIPT_NAME="] = _cgi_path;
	_env["SERVER_NAME="] = "ft_webserv";
	_env["SERVER_PORT="] = std::to_string(_req->getPort());
	_env["SERVER_PROTOCOL="] = "HTTP/1.1";
	_env["SERVER_SOFTWARE="] = "test_server";
	_env["REDIRECT_STATUS="] = "200";
	_env["REQUEST_URI="] = _req->getUrl().path + _req->getUrl().querry;

	if (_req->getMatchingLocation().getUploadPath() != "")
	{
		_env["UPLOAD_PATH="] = _req->getMatchingLocation().getUploadPath();
		_env["ROOT_PATH="] = _cur_dir + "/" + _req->getMatchingLocation().getRootPath();
	}

	if (DEBUG_MODE)
		for (t_strmap::iterator it = _env.begin(); it != _env.end(); it++)
			std::cout << it->first << it->second << " len: " << it->second.length() << std::endl;
}

// this is for testing need to make a proper version for this
char**	CgiHandler::makeEnvArray()
{
	char **envp;
	int	i = 0;

	envp = new char* [_env.size() + 1];

	for (t_strmap::iterator it = _env.begin(); it != _env.end(); it++, i++)
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
	_error = true;
	write(STDOUT_FILENO, "Status: 500 Internal Server Error\r\n", 35);
	exit(1);
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

std::string	CgiHandler::setResponseHeaders(std::string body)
{
	std::size_t	status = body.find("Status: ");
	if (status != std::string::npos)
		_req->setResponseStatus(body.substr(status + 8, body.find("\r\n", status + 8) - (status + 8)));
	else
		_req->setResponseStatus("200 Ok");
	std::size_t	content = body.find("Content-type:");
	if (content != std::string::npos)
		_req->setContentType(body.substr(content + 14, body.find("\n", content + 13) - content - 15));
	else
		_req->setContentType("application/octet-stream");
	std::size_t	sub = std::max<std::size_t>(content, status);
	if (sub == std::string::npos)
		sub = std::min<std::size_t>(content, status);
	if (sub != std::string::npos)
		body = body.substr(body.find("\r\n", sub) + 2);
	if (body == "Status: 500 Internal Server Error\r\n")
		body = "";
	return body;
}

std::string	CgiHandler::execute()
{
	char		**envp;
	pid_t		pid;

	setEnvValues();
	setFileDescriptors();
	if (_error)
		return setResponseHeaders("Status: 500 Internal Server Error\r\n");
	envp = makeEnvArray();
	
	//reading in the input body to temp file
	write(_in_file_fd, _input_body.c_str(), _input_body.length());
	//Resetting to beginning of file
	lseek(_in_file_fd, 0, SEEK_SET);

	//forking for executing script
	pid = fork();
	if (pid == -1)
		return setResponseHeaders("Status: 500 Internal Server Error\r\n");
	else if (pid == 0)
		executeScript(envp);
	else
		readScriptOutput(pid);

	closeFileDescriptors();

	//freeing envp
	for (std::size_t i = 0; envp[i]; i++)
		delete[] envp[i];
	delete envp;

	if (DEBUG_MODE)
		std::cout << "finished executing " << std::endl;
	return setResponseHeaders(_output_body);
}

