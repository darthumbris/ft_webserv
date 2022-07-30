#include "RequestHandler.hpp"


/*todo need to have a check here for post to check how big the upload is
// and compare it to the client_body_size variable of the server
*/

// Constructors
//
typedef struct	s_response
{
	std::string											protocal;
	std::vector<std::map<std::string, std::string>>		fields;

}				t_reponse;

RequestHandler::RequestHandler(Server *server) : _server(server)
{
}

RequestHandler::RequestHandler(const RequestHandler &copy)
{
	(void) copy;
}


// Destructor
RequestHandler::~RequestHandler()
{
}


// Operators
RequestHandler & RequestHandler::operator=(const RequestHandler &assign)
{
	(void) assign;
	return *this;
}

// Getters
std::string	RequestHandler::getResponse() const
{
	return this->_response;
}

//just a strncmp that cheks if line has GET POST DELETE as the starting charaters
bool	RequestHandler::isMethodImplimented(std::string line, std::string availableMethod)
{
	if (!line.compare(0, availableMethod.length(), availableMethod))
		return (true);
	return (false);
}

//check if the line is longer than mothod.length and if next char after is a space
bool	RequestHandler::isMethodFollowedBySpace(std::string line, std::string methodsIterator)
{
	if (methodsIterator.length() < line.length() && std::isspace(line.at(methodsIterator.length())))//checks if the next char after methodsIterator
		return (true);
	return (false);
}

bool	RequestHandler::parseFirstLine(std::string line)
{
	if (!line.empty())
	{
		for (std::vector<std::string>::const_iterator methodsIterator = _availableMethods.begin(); methodsIterator != _availableMethods.end(); methodsIterator++)
		{
			if (isMethodImplimented(line, *methodsIterator))
			{
				//method does not have anything else so there for its valid
				if (methodsIterator->length() == line.length())
				{
					_method = *methodsIterator;
					return (true);
				}
				if (isMethodFollowedBySpace(line, *methodsIterator))
				{
					_method = *methodsIterator;
					return (true);
				}
			}
		}
	}
	_method = "";
	return (false);
}

void	RequestHandler::setRequestMsg(std::string msg) 
{
	_msg = msg;

	std::vector<std::vector<std::string>>	request_line;
	request_line.push_back({"test", "GET"});
	std::cout << _protocol << std::endl;
	std::cout << msg << std::endl;

	std::string	line;
	std::istringstream iss(msg);

	if (parseFirstLine(msg))

	while (std::getline(iss, line))
	{
		request_line.push_back({"tes", line});
		/*
		try {
			if (line.compare(0, 3, "GET") == 0 )
				std::cout << "this is get rquest: " << line << std::endl;
			std::cout << "Hallo from loop: " << line << std::endl;
		}
		catch (std::exception& ex) {(void)ex;}
		*/
	}
	for (auto i = request_line.begin(); i < request_line.end(); i++)
	{
		for (auto j = i->begin(); j < i->end(); j++)
		{
			std::cout << "> " << *j;
		}
		std::cout << std::endl;
	}

	if (msg.find("\r\n\r\n"))
		std::cout << "end of request header" << std::endl;
}
