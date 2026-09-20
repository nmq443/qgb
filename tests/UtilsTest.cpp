#include <gtest/gtest.h>
#include "core/Utils.h"

using namespace qgb;

TEST(UtilsTest, MakeWordAndBytes)
{
    uint8_t high = 0x2c; // 0010'1100
    uint8_t low = 0x4a; // 0100'1010
    uint16_t word = makeWord(high, low);

    EXPECT_EQ(0x2c4a, word); // 0010'1100'0100'1010

    auto [actualHigh, actualLow] = makeBytes(word);
    EXPECT_EQ(actualHigh, high);
    EXPECT_EQ(actualLow, low);
}

TEST(UtilsTest, ReadGbRomSuccessfully)
{
    std::vector<uint8_t> rom;
    EXPECT_NO_THROW(rom = readGbRom("../roms/cpu_instrs/cpu_instrs.gb"));
}

TEST(UtilsTest, ReadGbRomFailed)
{
    std::vector<uint8_t> rom;
    EXPECT_ANY_THROW(rom = readGbRom(""));
}