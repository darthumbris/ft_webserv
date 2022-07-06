#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include <iostream>
# include <string>
# include "Server.hpp"

class RequestHandler
{
	public:
		// Constructors
		RequestHandler(Server *server);
		RequestHandler(const RequestHandler &copy);
		
		// Destructor
		~RequestHandler();
		
		// Operators
		RequestHandler & operator=(const RequestHandler &assign);
		
	private:
		Server	*_server;
};

#endif
