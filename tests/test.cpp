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

void	cpp_split(void)
{
	TEST_ASSERT_TRUE(request.cpp_split("test test test"));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(cpp_split);
    return UNITY_END();
}
