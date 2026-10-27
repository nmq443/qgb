#include "core/Utils.h"
#include <gtest/gtest.h>

using namespace qgb;

namespace
{
enum class TestFlags : uint8_t
{
    BitZero = 0,
    BitOne = 1,
    BitTwo = 2,
    BitSeven = 7
};
} // namespace

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

TEST(BitOperationsTest, GetAndSetBitsWithGenericEnum)
{
    uint8_t val = 0x00;

    // Initially all bits are false
    EXPECT_FALSE(getBit(val, TestFlags::BitZero));
    EXPECT_FALSE(getBit(val, TestFlags::BitSeven));

    // Turn on Bit Zero (0x01)
    val = setBit(val, TestFlags::BitZero, true);
    EXPECT_TRUE(getBit(val, TestFlags::BitZero));
    EXPECT_EQ(val, 0x01);

    // Turn on Bit Seven (0x80)
    val = setBit(val, TestFlags::BitSeven, true);
    EXPECT_TRUE(getBit(val, TestFlags::BitSeven));
    EXPECT_EQ(val, 0x81); // 0x01 | 0x80

    // Turn off Bit Zero
    val = setBit(val, TestFlags::BitZero, false);
    EXPECT_FALSE(getBit(val, TestFlags::BitZero));
    EXPECT_TRUE(getBit(val, TestFlags::BitSeven));
    EXPECT_EQ(val, 0x80);
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