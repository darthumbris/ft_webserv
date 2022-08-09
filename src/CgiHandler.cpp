#include "CgiHandler.hpp"

// Constructors
CgiHandler::CgiHandler(RequestHandler &req) : _req(req)
{
	_env["AUTH_TYPE="] = "";
	_env["CONTENT_LENGTH="] = std::to_string(_req.getResponseBody().length());
	_env["CONTENT_TYPE="] = "";
	_env["GATEWAY_INTERFACE="] = "CGI/1.1";
	_env["PATH_INFO="] = "";
	_env["PATH_TRANSLATED="] = "";
	_env["QUERY_STRING="] = "";
	_env["REMOTE_ADDR="] = "";
	_env["REMOTE_HOST="] = "";
	_env["REMOTE_IDENT="] = "";
	_env["REMOTE_USER="] = "";
	_env["REQUEST_METHOD="] = req.getRequestMethod();
	_env["SCRIPT_NAME="] = "";
	_env["SERVER_NAME="] = "";
	_env["SERVER_PORT="] = "";
	_env["SERVER_PROTOCOL="] = "HTTP/1.1";
	_env["SERVER_SOFTWARE="] = "";
	_env["REDIRECT_STATUS="] = "200";
	_env["SCRIPT_FILENAME="] = "";
	_env["REQUEST_URI="] = "";
	_cgi_path = "/Users/shoogenb/.brew/Cellar/php/8.1.9/bin/php-cgi"; //temp for now should get from config
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
		envp[i] = new char[it->first.size() + it->second.size() + 2];
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

