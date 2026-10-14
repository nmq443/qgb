#include <core/PPU.h>
#include <gtest/gtest.h>

using namespace qgb;

namespace // addresses of the ppu's registers
{
constexpr uint16_t kLcdc = 0xff40;
constexpr uint16_t kStat = 0xff41;
constexpr uint16_t kScy = 0xff42;
constexpr uint16_t kScx = 0xff43;
constexpr uint16_t kLy = 0xff44;
constexpr uint16_t kLyc = 0xff45;
constexpr uint16_t kDma = 0xff46;
constexpr uint16_t kBgp = 0xff47;
constexpr uint16_t kObp0 = 0xff48;
constexpr uint16_t kObp1 = 0xff49;
constexpr uint16_t kWy = 0xff4a;
constexpr uint16_t kWx = 0xff4b;

constexpr uint8_t kLcdOn = 0x80;
} // namespace

class PPUTest : public ::testing::Test
{
protected:
    void turnOn(uint8_t extraBits = 0x11)
    {
        ppu.write(kLcdc, kLcdOn | extraBits);
    }

    void turnOff()
    {
        ppu.write(kLcdc, 0x00);
    }

    PPUMode mode() const
    {
        return static_cast<PPUMode>(ppu.read(kStat) & 0x03);
    }
    uint8_t ly() const
    {
        return ppu.read(kLy);
    }

    PPU ppu;
};

TEST_F(PPUTest, PlainRegistersRoundTrip)
{
    const std::array<uint16_t, 9> regs = {kScy, kScx, kLyc, kDma, kBgp, kObp0, kObp1, kWy, kWx};
    for (auto reg : regs)
    {
        SCOPED_TRACE(reg);
        ppu.write(reg, 0xa5);
        EXPECT_EQ(ppu.read(reg), 0xa5);
        ppu.write(reg, 0x3c);
        EXPECT_EQ(ppu.read(reg), 0x3c);
    }
}

TEST_F(PPUTest, LcdcRoundTrip)
{
    ppu.write(kLcdc, 0x91);
    EXPECT_EQ(ppu.read(kLcdc), 0x91);
}

TEST_F(PPUTest, LyIsReadOnly)
{
    ppu.write(kLy, 0x42);
    EXPECT_EQ(ly(), 0);
}

TEST_F(PPUTest, UnmappedAddressReadsZero)
{
    EXPECT_EQ(ppu.read(0xff4c), 0);
    EXPECT_EQ(ppu.read(0x0000), 0);
}

TEST_F(PPUTest, VramReadWriteIncludingBoundaries)
{
    ppu.write(0x8000, 0x11);
    ppu.write(0x9fff, 0x22);
    ppu.write(0x9000, 0x33);
    EXPECT_EQ(ppu.read(0x8000), 0x11);
    EXPECT_EQ(ppu.read(0x9fff), 0x22);
    EXPECT_EQ(ppu.read(0x9000), 0x33);
}

TEST_F(PPUTest, OamReadWriteIncludingBoundaries)
{
    ppu.write(0xfe00, 0x44);
    ppu.write(0xfe9f, 0x55);
    EXPECT_EQ(ppu.read(0xfe00), 0x44);
    EXPECT_EQ(ppu.read(0xfe9f), 0x55);
}

TEST_F(PPUTest, WritesJustOutsideVramAndOamDoNotCorruptThem)
{
    ppu.write(0x7fff, 0xff);
    ppu.write(0xa000, 0xff);
    ppu.write(0xfdff, 0xff);
    ppu.write(0xfea0, 0xff);
    EXPECT_EQ(ppu.read(0x8000), 0x00);
    EXPECT_EQ(ppu.read(0x9fff), 0x00);
    EXPECT_EQ(ppu.read(0xfe00), 0x00);
    EXPECT_EQ(ppu.read(0xfe9f), 0x00);
}

TEST_F(PPUTest, VramAndOamAccessibleWhileLcdOff)
{
    turnOff();
    ppu.write(0x8123, 0xab);
    ppu.write(0xfe10, 0xcd);
    EXPECT_EQ(ppu.read(0x8123), 0xab);
    EXPECT_EQ(ppu.read(0xfe10), 0xcd);
}

TEST_F(PPUTest, StatBit7AlwaysReadsOne)
{
    EXPECT_TRUE(ppu.read(kStat) & 0x80);
    ppu.write(kStat, 0x00);
    EXPECT_TRUE(ppu.read(kStat) & 0x80);
}

TEST_F(PPUTest, StatOnlyBits3To6AreWritable)
{
    // Try to set all bits to 1. Only bits 3-6 should be writable,
    // bit 7 always reads as 1, and mode/coincidence bits (0-2) are read-only.
    ppu.write(kStat, 0xff);
    uint8_t stat = ppu.read(kStat);
    EXPECT_EQ(stat & 0xf8, 0xf8);                     // Bits 3-7
    EXPECT_EQ(stat & 0x03, static_cast<int>(mode())); // Bits 0-1 (PPU mode)

    // Try to clear bits 3-6.
    ppu.write(kStat, 0x00);
    EXPECT_EQ(ppu.read(kStat) & 0x78, 0x00); // Bits 3-6 should be cleared
}

TEST_F(PPUTest, StatLycFlagFollowsComparison)
{
    turnOn();
    ppu.write(kLyc, 1);
    EXPECT_FALSE(ppu.read(kStat) & 0x04); // LY=0, LYC=1

    ppu.tick(PPU::kDotsPerLine); // LY becomes 1
    EXPECT_EQ(ly(), 1);
    EXPECT_TRUE(ppu.read(kStat) & 0x04); // LY should equals LYC and set 2nd flag of STAT

    ppu.tick(PPU::kDotsPerLine);          // LY becomes 2
    EXPECT_FALSE(ppu.read(kStat) & 0x04); // LY=2, LYC=1
}