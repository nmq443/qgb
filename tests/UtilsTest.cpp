#include <gtest/gtest.h>
#include "core/Utils.h"

using namespace qgb;

TEST(UtilsTest, MakeWord) 
{
    uint8_t high = 0x2c; // 0010'1100
    uint8_t low = 0x4a; // 0100'1010
    uint16_t word = makeWord(low, high);
    EXPECT_EQ(0x2c4a, word); // 0010'1100'0100'1010
}