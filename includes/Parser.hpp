#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>

class Parser
{
	public:
		// Constructors
		Parser();
		Parser(const Parser &copy);
		
		// Destructor
		~Parser();
		
		// Operators
		Parser & operator=(const Parser &assign);
		
	private:
		
};

#endif