#include "unity.h"
#include <string.h>
#include "Parse.hpp"
#include "Json.hpp"
#include "Utils.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "WebServ.hpp"
#include "RequestHandler.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdbool.h>

void setUp(void)
{
}

void tearDown(void)
{
    // clean stuff up here
}

/*
void	access_test(void)
{
	int				_fd_response;
	std::string		_uri;
	std::ifstream	stream;

	_uri = "test.text";
	_fd_response = access(_uri.c_str(), F_OK); //checks if file exists
	stream.open(_uri);
	std::cout << _fd_response << stream.good() << std::endl;
	TEST_ASSERT_TRUE(_fd_response == stream.good());

	_fd_response = access(_uri.c_str(), R_OK); //checks if file is redable
	_fd_response = access(_uri.c_str(), W_OK); //checks if file is writeble
	stream.close();
}
*/

void	TESTLengthOfMatch(void)
{
//	std::cout << LengthOfMatch("var/html/test", "var/html/test") << " " << strlen("var/html/test") << std::endl;
	TEST_ASSERT_TRUE(LengthOfMatch("var/html/test", "var/html/test") == strlen("var/html/test"));
	TEST_ASSERT_TRUE(LengthOfMatch("/", "/") == strlen("/"));
	TEST_ASSERT_TRUE(LengthOfMatch("var", "var/html") == strlen("var"));
	TEST_ASSERT_TRUE(LengthOfMatch("var/html", "var") == strlen("var"));
}

void	TESTstripExesSlashes(void)
{
	TEST_ASSERT_TRUE("/" == stripExesSlashes("/"));
	TEST_ASSERT_TRUE("/test/html" == stripExesSlashes("/test///html"));
	TEST_ASSERT_TRUE("/" == stripExesSlashes("////////"));
	TEST_ASSERT_TRUE("/halo/test/test/" == stripExesSlashes("/halo//test/////test//"));
}

void	TESTParseRequestLine(void)
{
	std::string path;
	path = "configs/default.json";
	std::ifstream file(path);
	Parse parse;
	Json* json = parse.parse(file);
	Config config(json);

	RequestHandler req(config.getServerMap());
	req.setRequestMsg("GET / HTTP/1.1\r\nHost: test.com\r\nConnection: close\r\n\r\n");
	req.ParseRequestLine();
	std::cout << req
}

int main(void)
{
    UNITY_BEGIN();
    //RUN_TEST(access_test);
	RUN_TEST(TESTstripExesSlashes);
	RUN_TEST(TESTLengthOfMatch);
	RUN_TEST(TESTParseRequestLine);
    return UNITY_END();
}
