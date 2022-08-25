#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <iostream>
# include <string>
# include <unistd.h>
# include <map>
# include "RequestHandler.hpp"

class CgiHandler
{
	public:
		// Constructors
		CgiHandler(RequestHandler &req);
		
		// Destructor
		~CgiHandler();

		// Setter
		void	setCgiPaths();
		void	setEnvValues();
		std::string	setResponseHeaders(std::string body);

		// Member Functions
		std::string	execute();
		void	readScriptOutput(pid_t pid);
		void	executeScript(char **envp);
		void	setFileDescriptors();
		void	closeFileDescriptors();
		char**	makeEnvArray();
		
	private:
		
		RequestHandler	*_req;
		t_strmap		_env;
		std::string		_output_body;
		std::string		_cgi_path;
		int				_temp_in_fd;
		int				_temp_out_fd;
		int				_in_file_fd;
		int				_out_file_fd;
		FILE*			_tmp_in_file;
		FILE*			_tmp_out_file;
		// std::string		_folder;
		// std::string		_root;
		std::string		_file;
		std::string		_cur_dir;
		std::string		_input_body;
		bool			_error;
};

#endif
