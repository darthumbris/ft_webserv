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

#include <ctime>
#include <iostream>
#include <unistd.h>

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

void setUp(void)
{
}

void tearDown(void)
{
}

void	TEST_LengthOfMatch(void)
{
	TEST_ASSERT_TRUE(LengthOfMatch("var/html/test", "var/html/test") == strlen("var/html/test"));
	TEST_ASSERT_TRUE(LengthOfMatch("/", "/") == strlen("/"));
	TEST_ASSERT_TRUE(LengthOfMatch("var", "var/html") == strlen("var"));
	TEST_ASSERT_TRUE(LengthOfMatch("var/html", "var") == strlen("var"));
}

void	TEST_HexToStr(void)
{
	TEST_ASSERT_TRUE("ahahahahhaha" == HexToStr("%61%68%61%68%61%68%61%68%68%61%68%61"));
	TEST_ASSERT_TRUE("index. html" == HexToStr("index.%20html"));
	TEST_ASSERT_TRUE("///index. html" == HexToStr("%2f%2f%2findex.%20html"));
	TEST_ASSERT_TRUE("index. html" == HexToStr("%07%07%07index.%20html"));
	TEST_ASSERT_TRUE("///" == HexToStr("%2f%2f%2f"));
}

void	TEST_ParseRequestLine(void)
{
	int	x = 100;

	std::string path;
	path = "configs/default.json";
	std::ifstream file(path);
	Parse parse;
	Json* json = parse.parse(file);
	Config config(json);

	std::vector<std::string>	m = {"GET", "POST", "DELETE"};

	while (x)
	{
		RequestHandler	*req = new RequestHandler(config.getServerMap());

		std::string rm = m[rand() % 2];
		std::string rp = gen_random(100);
		req->setCompeleteRequest(rm + " /" + rp + " HTTP/1.1\r\n\r\n");
		req->ParseRequestLine();

		TEST_ASSERT_TRUE(req->getRequestMethod() == rm);
		TEST_ASSERT_TRUE(req->getUri() == "/" + rp);
		TEST_ASSERT_TRUE(req->getRequestProtocol() == "HTTP/1.1");
		x--;
		delete req;
	}
}

void	TEST_FindServer_WrongPort(void)
{
	//woulbe be nice to pass here some specific locatoins
	int	x = 100;
	std::string path;
	path = "configs/default.json";
	std::ifstream file(path);
	Parse parse;
	Json* json = parse.parse(file);
	Config config(json);

	while (x == -1)
	{
		RequestHandler *req = new RequestHandler(config.getServerMap());
		req->setCompeleteRequest("GET / HTTP/1.1\r\nHost: " + gen_random(x) +  "\r\n\n\n");
		req->setPort(123);
		req->makeHeaderMap();
		req->ParseHeaderMap();
		req->ParseRequestLine();
		req->FindServer();

		TEST_ASSERT_TRUE(req->getHost() == "");
		x--;
		delete req;
	}
}

void	TEST_FindServer_RightPort_NoHost(void)
{
	std::string path;
	path = "configs/default.json";
	std::ifstream file(path);
	Parse parse;
	Json* json = parse.parse(file);
	Config config(json);

	RequestHandler *req = new RequestHandler(config.getServerMap());
	req->setCompeleteRequest("GET / HTTP/1.1\r\n\n\n");
	req->setPort(4242);
	req->makeHeaderMap();
	req->ParseHeaderMap();
	req->ParseRequestLine();
	req->FindServer();

	TEST_ASSERT_TRUE(req->getHost() == "test.com");
	delete req;
}

/*
 *	lets say we find a server because there is a server listening to that port
 *
 */
void	TEST_FindServer_RightPort_Host(void)
{
	std::string path;
	path = "configs/default.json";
	std::ifstream file(path);
	Parse parse;
	Json* json = parse.parse(file);
	Config config(json);

	RequestHandler *req = new RequestHandler(config.getServerMap());
	req->setCompeleteRequest("GET / HTTP/1.1\r\nHost: test2.com\n\n");
	req->setPort(4242);
	req->makeHeaderMap();
	req->ParseHeaderMap();
	req->ParseRequestLine();
	req->FindServer();

	TEST_ASSERT_TRUE(req->getHost() == "test2.com");
	delete req;
	req = new RequestHandler(config.getServerMap());
	req->setCompeleteRequest("GET / HTTP/1.1\r\nHost: test3.com\n\n");
	req->setPort(4242);
	req->makeHeaderMap();
	req->ParseHeaderMap();
	req->ParseRequestLine();
	req->FindServer();

	TEST_ASSERT_TRUE(req->getHost() == "test3.com");
	delete req;
	req = new RequestHandler(config.getServerMap());
	req->setCompeleteRequest("GET / HTTP/1.1\r\nHost: other3.com\n\n");
	req->setPort(8080);
	req->makeHeaderMap();
	req->ParseHeaderMap();
	req->ParseRequestLine();
	req->FindServer();

	TEST_ASSERT_TRUE(req->getHost() == "other3.com");
	delete req;
}

void	TEST_FindTheRightLocationForUri(void)
{
	std::string path;
	path = "configs/default.json";
	std::ifstream file(path);
	Parse parse;
	Json* json = parse.parse(file);
	Config config(json);

	RequestHandler *req = new RequestHandler(config.getServerMap());

	req->setCompeleteRequest("GET /test/ HTTP/1.1\r\nHost: other3.com\n\n");
	req->setPort(8080);
	req->makeHeaderMap();
	req->ParseHeaderMap();
	req->ParseRequestLine();
	req->FindServer();
	req->FindTheRightLocationForUri();

	//TEST_ASSERT_TRUE(req->getMatchingDir() == "/");
	delete req;
}

void	TEST_FindTheRightLocationForUri2(void)
{
	std::string path;
	path = "configs/default.json";
	std::ifstream file(path);
	Parse parse;
	Json* json = parse.parse(file);
	Config config(json);

	RequestHandler *req = new RequestHandler(config.getServerMap());

	req->setCompeleteRequest("GET /test/./././../ HTTP/1.1\r\nHost: other3.com\r\n\r\n");

	req->setPort(8080);
	req->makeHeaderMap();
	req->ParseHeaderMap();
	req->ParseRequestLine();
	req->FindServer();
	req->FindTheRightLocationForUri();

	//std::cout << "I am the right loc: " << req->getMatchingDir() << std::endl;

	delete req;
}

int main(void)
{
    srand((unsigned)time(NULL) * getpid());     
    UNITY_BEGIN();
	//util fucntoins
	RUN_TEST(TEST_LengthOfMatch);
	RUN_TEST(TEST_HexToStr);
	//member functions fucntoins
	RUN_TEST(TEST_ParseRequestLine);
	RUN_TEST(TEST_FindServer_WrongPort);
	RUN_TEST(TEST_FindServer_RightPort_NoHost);
	RUN_TEST(TEST_FindServer_RightPort_Host);
	RUN_TEST(TEST_FindTheRightLocationForUri);
	RUN_TEST(TEST_FindTheRightLocationForUri2);
    return UNITY_END();
}
