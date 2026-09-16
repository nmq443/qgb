#include <gtest/gtest.h>
#include <core/Registers.h>

using namespace qgb;

class RegistersTest : public ::testing::Test
{
protected:
    Registers regs;
};

// Test initial reset state
TEST_F(RegistersTest, InitialStateIsZero) {
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