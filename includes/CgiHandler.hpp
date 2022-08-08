#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <iostream>
# include <string>
# include <unistd.h>

class CgiHandler
{
	public:
		// Constructors
		CgiHandler();
		CgiHandler(const CgiHandler &copy);
		
		// Destructor
		~CgiHandler();
		
		// Operators
		CgiHandler & operator=(const CgiHandler &assign);

		// Member Functions
		std::string	execute();
		
	private:
		
};

#endif
