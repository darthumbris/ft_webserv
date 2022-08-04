#include "unity.h"
#include "WebServ.hpp"
#include "Config.hpp"
#include <stdbool.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void isMethodImplimented(void) {
	Server serv;
	RequestHandler request(&serv);
	//const std::vector<std::string>	_availableMethods = {"GET", "POST", "DELETE"};

	TEST_ASSERT_FALSE(request.isMethodImplimented("G ", request._availableMethods[0]));
	TEST_ASSERT_TRUE(request.isMethodImplimented("GET ", request._availableMethods[0]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("get", request._availableMethods[0]));
	TEST_ASSERT_TRUE(request.isMethodImplimented("GET", request._availableMethods[0]));
	TEST_ASSERT_TRUE(request.isMethodImplimented("GETxczvsdf", request._availableMethods[0]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("asdfasfGETxczvsdf", request._availableMethods[0]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("", request._availableMethods[0]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("      GET ", request._availableMethods[0]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("asdfadsfhadsf", request._availableMethods[0]));

	TEST_ASSERT_TRUE(request.isMethodImplimented("POST", request._availableMethods[1]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("post", request._availableMethods[1]));
	TEST_ASSERT_TRUE(request.isMethodImplimented("POST ", request._availableMethods[1]));
	TEST_ASSERT_TRUE(request.isMethodImplimented("POSTxczvsdf", request._availableMethods[1]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("asdfasfPOSTxczvsdf", request._availableMethods[1]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("", request._availableMethods[1]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("      POST ", request._availableMethods[1]));

	TEST_ASSERT_TRUE(request.isMethodImplimented("DELETE", request._availableMethods[2]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("delete", request._availableMethods[2]));
	TEST_ASSERT_TRUE(request.isMethodImplimented("DELETE ", request._availableMethods[2]));
	TEST_ASSERT_TRUE(request.isMethodImplimented("DELETExczvsdf", request._availableMethods[2]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("asdfasfDELETExczvsdf", request._availableMethods[2]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("", request._availableMethods[2]));
	TEST_ASSERT_FALSE(request.isMethodImplimented("      DELETE ", request._availableMethods[2]));
}


void isMethodFollowedBySpace(void) {
	Server serv;
	RequestHandler request(&serv);

	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("GET ", request._availableMethods[0]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("GET\n", request._availableMethods[0]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("GET\t", request._availableMethods[0]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("GET\r", request._availableMethods[0]));

	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("POST ", request._availableMethods[1]));
	TEST_ASSERT_FALSE(request.isMethodFollowedBySpace("POST", request._availableMethods[1]));
	TEST_ASSERT_FALSE(request.isMethodFollowedBySpace(" POST", request._availableMethods[1]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("POST\n", request._availableMethods[1]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("POST\t", request._availableMethods[1]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("asdf\t", request._availableMethods[1]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("     ", request._availableMethods[1]));

	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("DELETE ", request._availableMethods[2]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("DELETE\t", request._availableMethods[2]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("DELETE\r", request._availableMethods[2]));
	TEST_ASSERT_TRUE(request.isMethodFollowedBySpace("DELETE\n", request._availableMethods[2]));
}

/*
 *
 * The Request-Line begins with a method token, followed by the Request-URI and the protocol version, and ending with CRLF. The elements are separated by SP characters. No CR or LF is allowed except in the final CRLF sequence.
 * RFC2616 24
 */

void	checkLine(void)
{
	TEST_ASSERT_TRUE(request.cpp_split(""));
	TEST_ASSERT_TRUE(request.cpp_split("\r\nadsfkadsfjlkj alsfjadksfjadlskfjas alsdfjadklsfj"));
	TEST_ASSERT_TRUE(request.cpp_split("\r\nadsfkadsfjlkj alsfjadksfjadlskfjas alsdfjadklsfj"));
}

void	parseFirstLine(void)
{
	Server			serv;
	RequestHandler	request(&serv);

	TEST_ASSERT_TRUE(request.parseFirstLine("GET / HTTP/1.1"));
	TEST_ASSERT_TRUE(request.parseFirstLine("POST / HTTP/1.1"));
	TEST_ASSERT_TRUE(request.parseFirstLine("DELETE / HTTP/1.1"));
	TEST_ASSERT_FALSE(request.parseFirstLine("get / HTTP/1.1"));
	TEST_ASSERT_FALSE(request.parseFirstLine("post / HTTP/1.1"));
	TEST_ASSERT_FALSE(request.parseFirstLine("delete / HTTP/1.1"));
	TEST_ASSERT_FALSE(request.parseFirstLine("GET/ HTTP/1.1"));
	TEST_ASSERT_FALSE(request.parseFirstLine("POST/ HTTP/1.1"));
	TEST_ASSERT_FALSE(request.parseFirstLine("DELETE/ HTTP/1.1"));
	TEST_ASSERT_FALSE(request.parseFirstLine(""));
	TEST_ASSERT_FALSE(request.parseFirstLine("asfadjsf;klwjfads;ij"));
	TEST_ASSERT_FALSE(request.parseFirstLine("GeT "));
	TEST_ASSERT_TRUE(request.parseFirstLine("GET"));
	TEST_ASSERT_TRUE(request.parseFirstLine("DELETE"));
	TEST_ASSERT_TRUE(request.parseFirstLine("POST"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(isMethodImplimented);
    RUN_TEST(isMethodFollowedBySpace);
    RUN_TEST(isMethodFollowedBySpace);
    RUN_TEST(parseFirstLine);
    return UNITY_END();
}
