#include "CgiHandler.hpp"

// Constructors
CgiHandler::CgiHandler(RequestHandler &req) : _req(req)
{
	_cgi_path = "/Users/shoogenb/.brew/Cellar/php/8.1.9/bin/php-cgi"; //temp for now should get from config
	_env["AUTH_TYPE="] = req.getHeaderMap()["Authorization"];
	_env["CONTENT_LENGTH="] = std::to_string(_req.getResponseBody().length());
	_env["CONTENT_TYPE="] = req.getHeaderMap()["content-type"];
	_env["GATEWAY_INTERFACE="] = "CGI/1.1";
	_env["PATH_INFO="] = req.getUrl().path;
	_env["PATH_TRANSLATED="] = req.getUrl().path;
	_env["QUERY_STRING="] = req.getUrl().querry;
	_env["REMOTE_ADDR="] = req.getClientIp();
	// _env["REMOTE_HOST="] = "";
	_env["REMOTE_IDENT="] = req.getHeaderMap()["Authorization"];
	_env["REMOTE_USER="] = req.getHeaderMap()["Authorization"];
	_env["REQUEST_METHOD="] = req.getRequestMethod();
	_env["SCRIPT_NAME="] = _cgi_path;
	_env["SERVER_NAME="] = req.getHeaderMap()["host"];
	_env["SERVER_PORT="] = std::to_string(req.getPort());
	_env["SERVER_PROTOCOL="] = "HTTP/1.1";
	_env["SERVER_SOFTWARE="] = "";
	_env["REDIRECT_STATUS="] = "200";
	_env["SCRIPT_FILENAME="] = _cgi_path;
	_env["REQUEST_URI="] = req.getUrl().path + req.getUrl().querry;
}


// Destructor
CgiHandler::~CgiHandler()
{
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
	char	buffer[256 + 1];
	int 	ret = 1;

	waitpid(pid, NULL, 0);
	lseek(_out_file_fd, 0, SEEK_SET);

	while (ret > 0)
	{
		ret = read(_out_file_fd, buffer, 256);
		buffer[ret] = '\0';
		_output_body.append(buffer); //this is the output of the script
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

//Executer needs some cleaning up and removing some test stuff but otherwise should be workable
std::string	CgiHandler::execute()
{
	char		**envp;
	pid_t		pid;
	std::string	input_body;

	setFileDescriptors();

	envp = makeEnvArray();

	//reading in the input body to temp file
	input_body = "name=test&email=kaas"; //this is what gets put as input for the script
	write(_in_file_fd, input_body.c_str(), input_body.length());
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

	return _output_body;
}

