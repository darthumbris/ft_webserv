#include "CgiHandler.hpp"

// Constructors
CgiHandler::CgiHandler()
{
}

CgiHandler::CgiHandler(const CgiHandler &copy)
{
	(void) copy;
}


// Destructor
CgiHandler::~CgiHandler()
{
}


// Operators
CgiHandler & CgiHandler::operator=(const CgiHandler &assign)
{
	(void) assign;
	return *this;
}

// this is for testing need to make a proper version for this
char	**getEnv()
{
	char **envp;

	envp = (char **)calloc(24, sizeof(char *));
	envp[0] = strdup("AUTH_TYPE=");
	envp[1] = strdup("REDIRECT_STATUS=200");
	envp[2] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	envp[3] = strdup("SCRIPT_NAME=subscription.php");
	envp[4] = strdup("SCRIPT_FILENAME=subscription.php");
	envp[5] = strdup("REQUEST_METHOD=POST");
	envp[6] = strdup("CONTENT_LENGTH=20");
	envp[7] = strdup("CONTENT_TYPE=application/x-www-form-urlencoded");
	envp[8] = strdup("PATH_INFO=/upload");
	envp[9] = strdup("PATH_TRANSLATED=/upload");
	envp[10] = strdup("QUERY_STRING=");
	envp[11] = strdup("REMOTEaddr=127.0.0.1");
	envp[12] = strdup("REMOTE_IDENT=auth");
	envp[13] = strdup("REMOTE_USER=remote_usr");
	envp[14] = strdup("REQUEST_URI=/upload");
	envp[15] = strdup("SERVER_NAME=127.0.0.1");
	envp[16] = strdup("SERVER_PORT=4242");
	envp[17] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	envp[18] = strdup("SERVER_SOFTWARE=test.com");
	envp[19] = strdup("name=test&email=kaas");
	envp[20] = NULL;
	return (envp);
}

//Executer needs some cleaning up and removing some test stuff but otherwise should be workable
std::string	CgiHandler::execute()
{
	char		**envp;
	int			tempIn = dup(STDIN_FILENO);
	int			tempOut = dup(STDOUT_FILENO);
	FILE		*tmpIn = tmpfile();
	FILE		*tmpOut = tmpfile();
	int			In = fileno(tmpIn);
	int			Out = fileno(tmpOut);
	pid_t		pid;
	std::string body;
	std::string	test;

	envp = getEnv();
	test = "name=test&email=kaas";
	write(In, test.c_str(), 20);
	lseek(In, 0, SEEK_SET);

	pid = fork();
	if (pid == -1)
		return ("Status: 500\r\n");
	else if (pid == 0)
	{
		dup2(In, STDIN_FILENO);
		dup2(Out, STDOUT_FILENO);
		execve("/Users/shoogenb/.brew/Cellar/php/8.1.9/bin/php-cgi", NULL, envp);
		write(STDOUT_FILENO, "Status: 500\r\n", 15);
	}
	else
	{
		char	buffer[256 + 1];
	
		waitpid(pid, NULL, 0);
		lseek(Out, 0, SEEK_SET);

		int ret = 1;
		while (ret > 0)
		{
			ret = read(Out, buffer, 256);
			buffer[ret] = '\0';
			body += buffer;
		}
	}
	

	dup2(tempIn, STDIN_FILENO);
	dup2(tempOut, STDOUT_FILENO);
	fclose(tmpIn);
	fclose(tmpOut);
	close(In);
	close(Out);
	close(tempIn);
	close(tempOut);

	if (pid == 0)
		exit(0);
	return body;
}

