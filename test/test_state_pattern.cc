#include <memory>
#include <gtest/gtest.h>
#include "faker_tsn.h"
#include "test.h"

using namespace faker_tsn;

void test_state_pattern() {
    TSNContext &context = TSNContext::getInstance();
    std::shared_ptr<ITSNContextState> initState = std::make_shared<InitTSNContextState>();
    std::shared_ptr<ITSNContextState> runState = std::make_shared<RunTSNContextState>();

    initState->doAction(context);
    runState->doAction(context);
}
TEST(TEST_STATE_PATTERN,TEST_STATE_PATTERN){
    test_state_pattern();
}