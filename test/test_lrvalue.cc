#include <iostream>
#include "faker_tsn.h"
#include "test.h"
#include <gtest/gtest.h>
void foo(int &a)
{
    std::cout << "int &: " << std::to_string(a) << std::endl;
}

void foo(int &&a)
{
    std::cout << "int &&: " << std::to_string(a) << std::endl;
}

void foo(const int &a)
{
    std::cout << "const int &: " << std::to_string(a) << std::endl;
}

void test_lrvalue()
{
    int a = 1;
    const int b = 1;
    foo(a);
    foo(std::move(a));
    foo(b);
}
TEST(TEST_LRVALUE, TEST_LRVALUE) {
test_lrvalue();
}