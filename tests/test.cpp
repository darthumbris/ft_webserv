#include "unity.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdbool.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(access_test);
    return UNITY_END();
}
