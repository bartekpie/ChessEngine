#include <gtest/gtest.h>
#include <iostream>
#include "move.hpp"

TEST(MoveTest, constructorTest) {
    Move m(static_cast<uint16_t>(1ULL << 2));
    EXPECT_EQ(m.from(), 0x4);
}
