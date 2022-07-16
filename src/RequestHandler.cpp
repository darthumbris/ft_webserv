#include "RequestHandler.hpp"


/*TODO need to have a check here for post to check how big the upload is
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

// Setters
void	RequestHandler::setRequestMsg(std::string msg) 
{
	_msg = msg;
	//std::cout << "< mes this is >" << msg;
	//TODO make sure to parse this message. 
	//TODO might need a need to see if the msg is done being received?
	// for request headers they always end with \r\n\r\n (section 4.1 of RFC 2616)
	// We can use that to check if the full message has been received.
	// if it has been fully received set a bool _is_fully_read or something to true?
	// might also work for sending the response?
	//reponse herader?
	std::cout << _protocol << std::endl;
	std::cout << msg << std::endl;

	std::string	line;
	std::istringstream iss(msg);
	while (std::getline(iss, line))
	{
		try {
			std::cout <<  << std::endl;
			if (line.compare(0, 3, "GET") == 0 && std::isspace(line.at(3)))
				std::cout << "this is get rquest: " << line << std::endl;
			std::cout << "Hallo from loop: " << line << std::endl;
		}
		catch (std::exception& ex) {(void)ex;}
	}

	if (msg.find("\r\n\r\n"))
		std::cout << "end of request header" << std::endl;
}
