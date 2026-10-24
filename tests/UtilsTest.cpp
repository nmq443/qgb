#include "core/Utils.h"
#include <gtest/gtest.h>

using namespace qgb;

TEST(UtilsTest, MakeWordAndBytes)
{
    uint8_t high = 0x2c; // 0010'1100
    uint8_t low = 0x4a;  // 0100'1010
    uint16_t word = makeWord(high, low);

    EXPECT_EQ(0x2c4a, word); // 0010'1100'0100'1010

    auto [actualHigh, actualLow] = makeBytes(word);
    EXPECT_EQ(actualHigh, high);
    EXPECT_EQ(actualLow, low);
}

TEST(UtilsTest, GetAndSetBits)
{
    // 1. Test getBit with a known value (0b00001010 / decimal 10)
    uint8_t num{0b00001010};
    EXPECT_TRUE(getBit(num, 1));
    EXPECT_TRUE(getBit(num, 3));
    EXPECT_FALSE(getBit(num, 0));
    EXPECT_FALSE(getBit(num, 2));

    // 2. Test setBit and verify modifications
    uint8_t val{0b00000000};

    // Set bit 0 to true
    val = setBit(val, 0, true);
    EXPECT_EQ(val, 0b00000001);
    EXPECT_TRUE(getBit(val, 0));

    // Set bit 3 to true
    val = setBit(val, 3, true);
    EXPECT_EQ(val, 0b00001001);
    EXPECT_TRUE(getBit(val, 3));

    // Clear bit 0 back to false
    val = setBit(val, 0, false);
    EXPECT_EQ(val, 0b00001000);
    EXPECT_FALSE(getBit(val, 0));
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