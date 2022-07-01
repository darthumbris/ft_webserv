#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>

class WebServ
{
	public:
		// Constructors
		WebServ();
		WebServ(const WebServ &copy);
		
		// Destructor
		~WebServ();
		
		// Operators
		WebServ & operator=(const WebServ &assign);
		
	private:
		
};

#endif