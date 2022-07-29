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

bool	RequestHandler::isMethodImplimented(std::string line, std::string availableMethod)
{
	if (!line.compare(0, availableMethod.length(), availableMethod))
		return (true);
	return (false);
}

void	RequestHandler::parseMethod(std::string line)
{
	for (std::vector<std::string>::const_iterator methodsIterator = _availableMethods.begin(); methodsIterator != _availableMethods.end(); methodsIterator++)
	{
		if (isMethodImplimented(line, *methodsIterator))
		{
			if (methodsIterator->length() < line.length() && std::isspace(line.at(methodsIterator->length())))//checks if the next char after methodsIterator
			{
				std::cout << "The right line found: "<< *methodsIterator << std::endl;
			}
		}
	}
}

void	RequestHandler::setRequestMsg(std::string msg) 
{
	_msg = msg;
	//std::cout << "< mes this is >" << msg;
	//todo make sure to parse this message. 
	//todo might need a need to see if the msg is done being received?
	// for request headers they always end with \r\n\r\n (section 4.1 of RFC 2616)
	// We can use that to check if the full message has been received.
	// if it has been fully received set a bool _is_fully_read or something to true?
	// might also work for sending the response?
	//reponse herader?
	std::vector<std::vector<std::string>>	request_line;
	request_line.push_back({"test", "GET"});
	std::cout << _protocol << std::endl;
	std::cout << msg << std::endl;

	std::string	line;
	std::istringstream iss(msg);
	parseMethod(msg);
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
