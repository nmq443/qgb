#include <core/Registers.h>
#include <gtest/gtest.h>

using namespace qgb;

class RegistersTest : public ::testing::Test
{
protected:
    Registers regs;
};

// Test initial reset state
TEST_F(RegistersTest, InitialStateIsZero)
{
    EXPECT_EQ(regs.getA(), 0);
    EXPECT_EQ(regs.getB(), 0);
    EXPECT_EQ(regs.getC(), 0);
    EXPECT_EQ(regs.getD(), 0);
    EXPECT_EQ(regs.getE(), 0);
    EXPECT_EQ(regs.getF(), 0);
    EXPECT_EQ(regs.getH(), 0);
    EXPECT_EQ(regs.getL(), 0);

    EXPECT_EQ(regs.getPC(), 0);
    EXPECT_EQ(regs.getSP(), 0);

    EXPECT_EQ(regs.getAF(), 0);
    EXPECT_EQ(regs.getBC(), 0);
    EXPECT_EQ(regs.getDE(), 0);
    EXPECT_EQ(regs.getHL(), 0);
}

// Test individual flag setting and getting
TEST_F(RegistersTest, FlagManipulation)
{
    // Test Zero Flag
    regs.setFlag(Flag::Zero, true);
    EXPECT_TRUE(regs.getFlag(Flag::Zero));

    regs.setFlag(Flag::Zero, false);
    EXPECT_FALSE(regs.getFlag(Flag::Zero));

    // Test Carry Flag
    regs.setFlag(Flag::Carry, true);
    EXPECT_TRUE(regs.getFlag(Flag::Carry));

    regs.setFlag(Flag::Carry, false);
    EXPECT_FALSE(regs.getFlag(Flag::Carry));

    // Test Subtraction and HalfCarry
    regs.setFlag(Flag::Subtraction, true);
    regs.setFlag(Flag::HalfCarry, true);

    EXPECT_TRUE(regs.getFlag(Flag::Subtraction));
    EXPECT_TRUE(regs.getFlag(Flag::HalfCarry));

    // Ensure independent flag operation
    regs.setFlag(Flag::Subtraction, false);
    EXPECT_FALSE(regs.getFlag(Flag::Subtraction));
    EXPECT_TRUE(regs.getFlag(Flag::HalfCarry));
}

TEST_F(RegistersTest, SetRegisterPair)
{
    // for AF register pair, when setting we set lower 4 bits of f register to 0
    regs.setAF(0x1234);
    EXPECT_EQ(regs.getA(), 0x12);
    EXPECT_EQ(regs.getF(), 0x30);
    EXPECT_EQ(regs.getAF(), 0x1230);

    regs.setBC(0x1234);
    EXPECT_EQ(regs.getB(), 0x12);
    EXPECT_EQ(regs.getC(), 0x34);
    EXPECT_EQ(regs.getBC(), 0x1234);

    regs.setDE(0x1234);
    EXPECT_EQ(regs.getD(), 0x12);
    EXPECT_EQ(regs.getE(), 0x34);
    EXPECT_EQ(regs.getDE(), 0x1234);

    regs.setHL(0x1234);
    EXPECT_EQ(regs.getH(), 0x12);
    EXPECT_EQ(regs.getL(), 0x34);
    EXPECT_EQ(regs.getHL(), 0x1234);
}