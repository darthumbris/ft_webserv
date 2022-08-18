#include "CgiHandler.hpp"
#include "Utils.hpp"

// Constructors
CgiHandler::CgiHandler(RequestHandler &req) : _req(&req)
{
	std::cout << "Cgi made" << std::endl;
	std::string	request = _req->getCompleteRequest();
	std::size_t	body_start = request.find("\r\n\r\n");
	_input_body = request.substr(body_start + 4);
}


// Destructor
CgiHandler::~CgiHandler()
{
}

// Setter
void	CgiHandler::setCgiPaths()
{
	_folder = _req->getUrl().path.substr(0, _req->getUrl().path.find_last_of('/') + 1);
	_file = _req->getUrl().path.substr(_req->getUrl().path.find_last_of('/') + 1, _req->getUrl().path.length());
	_root = "/";
	if (_req->getLocation(_folder))
		_root += _req->getLocation(_folder)->getRootPath();
	if (_req->getLocation(_folder))
		_cgi_path = _req->getLocation(_folder)->getCgiPath();
	else
		_cgi_path = ""; // TODO might need to give a server error in this case
	
	// std::cout << "root: " << _root << std::endl;
	// std::cout << "folder: " << _folder << std::endl;
	// std::cout << "file: " << _file << std::endl;
	_cur_dir = getCurDir();
}

//TODO for fileupload maybe have the redirect status be 303?
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
	_env["SERVER_NAME="] = "test_server"; // TODO get correct name from _req or from the headermap?
	_env["SERVER_PORT="] = std::to_string(_req->getPort());
	_env["SERVER_PROTOCOL="] = "HTTP/1.1";
	_env["SERVER_SOFTWARE="] = "test_server";
	_env["REDIRECT_STATUS="] = "200";
	_env["REQUEST_URI="] = _req->getUrl().path + _req->getUrl().querry;
	if (_req->getLocation(_folder)->getUploadPath() != "")
		_env["UPLOAD_PATH="] = _req->getLocation(_folder)->getUploadPath();
	// std::cout << "current dir:" << _cur_dir << std::endl;
	// std::cout << "path_info:" << _env["PATH_INFO="] << std::endl;
	// std::cout << "path_translated:" << _env["PATH_TRANSLATED="] << std::endl;
	// std::cout << "content_length: " << _env["CONTENT_LENGTH="] << std::endl;
	// std::cout << "REQUEST_METHOD: " << _env["REQUEST_METHOD="] << std::endl;
	for (auto it = _env.begin(); it != _env.end(); it++)
		std::cout << it->first << it->second << " len: " << it->second.length() << std::endl;


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
	_req->setCgiError();
	write(STDOUT_FILENO, "Status: 500\r\n", 14);
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

//TODO make sure to make the folder if request is post (look at the config for where to upload)
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
	{
		_req->setCgiError();
		return ("Status: 500\r\n");
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
	std::cout << "finished executing " << std::endl;

	//TODO check if this works always or needs to be done differently
	//TODO might need to remove this part later so the requesthandler can see what content-type to set for the response header
	std::size_t	start_content_type = _output_body.find("Content-type");
	std::size_t	start_body = _output_body.find('\n', start_content_type);
	std::cout << _output_body.substr(0, start_body);
	_output_body = _output_body.substr(start_body, std::string::npos);
	// std::cout << "output: " << _output_body << std::endl;
	return _output_body;
}

