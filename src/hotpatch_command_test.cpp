#include <iostream>
#include <gtest/gtest.h>

#include "hotpatch_command.h"

using namespace std;
using namespace hotpatch;

TEST(HotpatchCommand, Parse) {
    EXPECT_EQ(15, 15);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}