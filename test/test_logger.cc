#include "faker_tsn.h"
#include "test.h"
#include <faker_tsn.h>
#include <gtest/gtest.h>
using namespace faker_tsn;

void test_logger() {
    Log::setLoggerLevel(Log::INFO_LEVEL);
    DEBUG("debug world");
    INFO("info world");
}
TEST(TEST_LOGGER, TEST_LOGGER) {
test_logger();
}