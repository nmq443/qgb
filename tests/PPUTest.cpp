#include <algorithm>
#include <core/PPU.h>
#include <core/Utils.h>
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

TEST_F(PPUTest, TickDoesNothingWhileLcdOff)
{
    turnOff();
    ppu.tick(PPU::kDotsPerLine * 10);
    EXPECT_EQ(ly(), 0);
    EXPECT_EQ(mode(), PPUMode::HBlank);
}

TEST_F(PPUTest, TurningLcdOffResetsLyAndMode)
{
    turnOn();
    ppu.tick(PPU::kDotsPerLine * 5 + 100); // LY=5, somewhere in drawing
    ASSERT_EQ(ly(), 5);

    turnOff();
    EXPECT_EQ(ly(), 0);
    EXPECT_EQ(mode(), PPUMode::HBlank);
    EXPECT_EQ(ppu.read(kLcdc) & 0x80, 0); // bit 7 reads back as 0
}

TEST_F(PPUTest, TurningLcdOffDoesNotClearStatInterruptEnables)
{
    // On hardware only the mode / LYC flag are reset; STAT enable bits are
    // plain game-written state.
    turnOn();
    ppu.write(kStat, 0x78);
    turnOff();
    EXPECT_EQ(ppu.read(kStat) & 0x78, 0x78);
}

TEST_F(PPUTest, TurningLcdOffClearsFrameBuffer)
{
    turnOn();
    ppu.tick(PPU::kDotsPerLine * 3);
    turnOff();
    const auto &fb = ppu.getFrameBuffer();
    EXPECT_TRUE(std::all_of(fb.begin(), fb.end(), [](uint8_t p)
                            { return p == 0; }));
}

TEST_F(PPUTest, TurningLcdOnRestartsAtLineZeroDotZero)
{
    turnOn();
    ppu.tick(PPU::kDotsPerLine * 3 + 200);
    turnOff();
    turnOn();
    EXPECT_EQ(ly(), 0);

    // if mDot was properly reset, the first mode change is exactly at dot 80
    ppu.tick(PPU::kOamScanDots - 1);
    EXPECT_EQ(mode(), PPUMode::OamScan);
    ppu.tick(1);
    EXPECT_EQ(mode(), PPUMode::Drawing);
}

TEST_F(PPUTest, WritingLcdcWithBit7StillSetDoesNotRestartPpu)
{
    turnOn(0x01);
    ppu.tick(PPU::kDotsPerLine * 2);
    ASSERT_EQ(ly(), 2);
    ppu.write(kLcdc, kLcdOn | 0x10); // change other bits, LCD stays on
    EXPECT_EQ(ly(), 2);
}

TEST_F(PPUTest, ModeSequenceWithinVisibleLine)
{
    turnOn();
    EXPECT_EQ(mode(), PPUMode::OamScan);

    ppu.tick(PPU::kOamScanDots - 1);
    EXPECT_EQ(mode(), PPUMode::OamScan);
    ppu.tick(1);
    EXPECT_EQ(mode(), PPUMode::Drawing);

    ppu.tick(PPU::kDrawingDots - 1);
    EXPECT_EQ(mode(), PPUMode::Drawing);
    ppu.tick(1);
    EXPECT_EQ(mode(), PPUMode::HBlank);

    ppu.tick(PPU::kDotsPerLine - PPU::kOamScanDots - PPU::kDrawingDots - 1);
    EXPECT_EQ(mode(), PPUMode::HBlank);
    EXPECT_EQ(ly(), 0);

    ppu.tick(1); // end of line
    EXPECT_EQ(ly(), 1);
    EXPECT_EQ(mode(), PPUMode::OamScan);
}

TEST_F(PPUTest, EntersVBlankAtLine144)
{
    turnOn();
    ppu.tick(PPU::kDotsPerLine * PPU::kVBlankStart - 1);
    EXPECT_EQ(ly(), 143);
    EXPECT_EQ(mode(), PPUMode::HBlank);

    ppu.tick(1);
    EXPECT_EQ(ly(), 144);
    EXPECT_EQ(mode(), PPUMode::VBlank);
}

TEST_F(PPUTest, StaysInVBlankThroughLine153)
{
    turnOn();
    ppu.tick(PPU::kVBlankStart * PPU::kDotsPerLine); // Fast-forward to line 144
    for (int line = PPU::kVBlankStart; line < PPU::kLinesPerFrame; ++line)
    {
        SCOPED_TRACE(line);
        ASSERT_EQ(ly(), line);
        // sample at several points of the line; mode must never change
        EXPECT_EQ(mode(), PPUMode::VBlank);
        ppu.tick(PPU::kOamScanDots);
        EXPECT_EQ(mode(), PPUMode::VBlank);
        ppu.tick(PPU::kDotsPerLine - PPU::kOamScanDots - 1);
        EXPECT_EQ(mode(), PPUMode::VBlank);
        ppu.tick(1);
    }
}

TEST_F(PPUTest, FrameWrapsBackToLineZero)
{
    turnOn();
    ppu.tick(PPU::kDotsPerLine * PPU::kLinesPerFrame - 1);
    EXPECT_EQ(ly(), 153);
    ppu.tick(1);
    EXPECT_EQ(ly(), 0);
    EXPECT_EQ(mode(), PPUMode::OamScan);
}

TEST_F(PPUTest, TwoFramesBehaveIdentically)
{
    turnOn();
    ppu.tick(PPU::kDotsPerLine * PPU::kLinesPerFrame * 2 + 10);
    EXPECT_EQ(ly(), 0);
    EXPECT_EQ(mode(), PPUMode::OamScan);
}

TEST_F(PPUTest, TickInSmallStepsMatchesOneBigTick)
{
    PPU other;
    other.write(kLcdc, 0x91);
    turnOn(0x11);

    const int total = PPU::kDotsPerLine * 50 + 123;
    ppu.tick(total);
    for (int i = 0; i < total; ++i)
    {
        other.tick(1);
    }
    EXPECT_EQ(ppu.read(kLy), other.read(kLy));
    EXPECT_EQ(ppu.read(kStat), other.read(kStat));
}

TEST_F(PPUTest, TickZeroIsNoOp)
{
    turnOn();
    ppu.tick(0);
    EXPECT_EQ(ly(), 0);
    EXPECT_EQ(mode(), PPUMode::OamScan);
}

TEST_F(PPUTest, FrameReadyFlagGetterSetter)
{
    EXPECT_FALSE(ppu.isFrameReady());
    ppu.setFrameReady(true);
    EXPECT_TRUE(ppu.isFrameReady());
    ppu.setFrameReady(false);
    EXPECT_FALSE(ppu.isFrameReady());
}

TEST_F(PPUTest, FrameBufferHasExpectedSize)
{
    EXPECT_EQ(ppu.getFrameBuffer().size(),
              static_cast<size_t>(PPU::kLcdWidth * PPU::kLcdHeight));
}

TEST_F(PPUTest, FrameReadyIsSetWhenVBlankStarts)
{
    turnOn();
    ppu.tick(PPU::kDotsPerLine * PPU::kVBlankStart - 1);
    EXPECT_FALSE(ppu.isFrameReady());
    ppu.tick(1);
    EXPECT_TRUE(ppu.isFrameReady());
}

TEST_F(PPUTest, StatInterruptFiresOnLycMatch)
{
    int count = 0;
    ppu.setStatInterrupt([&]
                         { ++count; });
    turnOn();
    ppu.write(kStat, 0x40); // LYC interrupt enable
    ppu.write(kLyc, 2);

    ppu.tick(PPU::kDotsPerLine * 2);
    EXPECT_EQ(count, 1);
    ppu.tick(PPU::kDotsPerLine); // LY=3, no new edge
    EXPECT_EQ(count, 1);
}

TEST_F(PPUTest, StatInterruptFiresOnHBlank)
{
    int count = 0;
    ppu.setStatInterrupt([&]
                         { ++count; });
    turnOn();
    ppu.write(kStat, 0x08); // mode 0 interrupt enable
    ppu.tick(PPU::kOamScanDots + PPU::kDrawingDots);
    EXPECT_EQ(count, 1);
}

TEST_F(PPUTest, StatInterruptIsEdgeTriggered)
{
    int count = 0;
    ppu.setStatInterrupt([&]
                         { ++count; });
    turnOn();
    ppu.write(kStat, 0x10);                            // VBlank interrupt enable
    ppu.tick(PPU::kDotsPerLine * PPU::kLinesPerFrame); // whole VBlank period
    EXPECT_EQ(count, 1);                               // fires once on entering VBlank, not every tick
}

TEST_F(PPUTest, NullStatCallbackDoesNotCrash)
{
    // this test exists because i do not wire any stat callback into ppu
    turnOn();
    ppu.write(kStat, 0x78);
    EXPECT_NO_FATAL_FAILURE(ppu.tick(PPU::kDotsPerLine * PPU::kLinesPerFrame));
}

TEST_F(PPUTest, AllZerosResultsInBlankTile)
{
    uint16_t tileAddr = 0x8000;
    for (int i = 0; i < 16; ++i)
    {
        ppu.write(tileAddr + i, 0); // Fill 16 bytes with zeros in VRAM
    }

    Tile tile{ppu.decodeTile(tileAddr)};

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            EXPECT_EQ(tile[row][col], 0);
        }
    }
}

TEST_F(PPUTest, AllOnesResultsInMaxTileValue)
{
    uint16_t tileAddr = 0x8000;
    for (int i = 0; i < 16; ++i)
    {
        ppu.write(tileAddr + i, 0xFF); // All bits set to 1
    }

    Tile tile{ppu.decodeTile(tileAddr)};

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            EXPECT_EQ(tile[row][col], 3); // 0b0000'0011
        }
    }
}

TEST_F(PPUTest, CorrectBitPlaneCombination)
{
    uint16_t tileAddr = 0x8000;
    for (int i = 0; i < 16; ++i)
    {
        ppu.write(tileAddr + i, 0); // Clear tile bytes
    }

    // Set up Row 0 only:
    ppu.write(tileAddr + 0, 0xAA); // lo = 0xAA (binary: 10101010)
    ppu.write(tileAddr + 1, 0x55); // hi = 0x55 (binary: 01010101)

    Tile tile{ppu.decodeTile(tileAddr)};

    EXPECT_EQ(tile[0][0], 1); // bit 7: hi=0, lo=1
    EXPECT_EQ(tile[0][1], 2); // bit 6: hi=1, lo=0
    EXPECT_EQ(tile[0][2], 1); // bit 5: hi=0, lo=1
    EXPECT_EQ(tile[0][3], 2); // bit 4: hi=1, lo=0

    // Unmodified rows should remain 0
    for (int col = 0; col < 8; ++col)
    {
        EXPECT_EQ(tile[1][col], 0);
    }
}

TEST_F(PPUTest, TileDataAddressUnsignedMode_0x8000)
{
    // 0x8000 - 0x8FFF range
    turnOn(setBit(1, static_cast<uint8_t>(LCDC::BgAndWindowTiles), true));

    EXPECT_EQ(ppu.getTileDataAddress(0), 0x8000);
    EXPECT_EQ(ppu.getTileDataAddress(1), 0x8010);
    EXPECT_EQ(ppu.getTileDataAddress(255), 0x8FF0);
}

TEST_F(PPUTest, TileDataAddressSignedMode_0x8800)
{
    // 0x8800 - 0x97FF range
    turnOn(setBit(1, static_cast<uint8_t>(LCDC::BgAndWindowTiles), false));

    EXPECT_EQ(ppu.getTileDataAddress(0), 0x9000);
    EXPECT_EQ(ppu.getTileDataAddress(1), 0x9010);
    EXPECT_EQ(ppu.getTileDataAddress(127), 0x97F0);                       // Max positive offset
    EXPECT_EQ(ppu.getTileDataAddress(static_cast<int8_t>(-1)), 0x8FF0);   // -1 wraps into 0x8800 block
    EXPECT_EQ(ppu.getTileDataAddress(static_cast<int8_t>(-128)), 0x8800); // Min negative offset
}