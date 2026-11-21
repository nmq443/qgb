#include <core/APU.h>
#include <gtest/gtest.h>
#include <vector>

using namespace qgb;

class APUTest : public ::testing::Test
{
protected:
    void powerOn()
    {
        apu.write(APU::kNR52, 0b1000'0000);
    }

    void powerOff()
    {
        apu.write(APU::kNR52, 0x00);
    }

    APU apu;
};

// All real registers except NR52 (which has special behavior).
const std::vector<uint16_t> kRegisters = {
    APU::kNR10,
    APU::kNR11,
    APU::kNR12,
    APU::kNR13,
    APU::kNR14,
    APU::kNR21,
    APU::kNR22,
    APU::kNR23,
    APU::kNR24,
    APU::kNR30,
    APU::kNR31,
    APU::kNR32,
    APU::kNR33,
    APU::kNR34,
    APU::kNR41,
    APU::kNR42,
    APU::kNR43,
    APU::kNR44,
    APU::kNR50,
    APU::kNR51,
};

class APUReadMaskTest : public APUTest, public ::testing::WithParamInterface<uint16_t>
{
};

TEST_P(APUReadMaskTest, WritingZeroReadsBackMask)
{
    powerOn();
    const auto addr = GetParam();
    apu.write(addr, 0x00);
    EXPECT_EQ(apu.read(addr), APU::kReadMask[addr - 0xff10]) << std::hex << "addr=0x" << addr;
}

INSTANTIATE_TEST_SUITE_P(AllRegisters, APUReadMaskTest, ::testing::ValuesIn(kRegisters));

TEST_F(APUTest, SpecificMasks)
{
    powerOn();
    apu.write(APU::kNR10, 0x00);
    EXPECT_EQ(apu.read(APU::kNR10), 0x80);

    apu.write(APU::kNR13, 0x12); // write-only
    EXPECT_EQ(apu.read(APU::kNR13), 0xff);

    apu.write(APU::kNR14, 0x00);
    EXPECT_EQ(apu.read(APU::kNR14), 0xbf);

    apu.write(APU::kNR30, 0x00);
    EXPECT_EQ(apu.read(APU::kNR30), 0x7f);

    apu.write(APU::kNR32, 0x00);
    EXPECT_EQ(apu.read(APU::kNR32), 0x9f);
}

TEST_F(APUTest, FullyReadableRegistersRoundTrip)
{
    powerOn();
    apu.write(APU::kNR12, 0x5a);
    apu.write(APU::kNR22, 0xa5);
    apu.write(APU::kNR42, 0x3c);
    apu.write(APU::kNR43, 0xc3);
    apu.write(APU::kNR50, 0x77);
    apu.write(APU::kNR51, 0xf3);

    EXPECT_EQ(apu.read(APU::kNR12), 0x5a);
    EXPECT_EQ(apu.read(APU::kNR22), 0xa5);
    EXPECT_EQ(apu.read(APU::kNR42), 0x3c);
    EXPECT_EQ(apu.read(APU::kNR43), 0xc3);
    EXPECT_EQ(apu.read(APU::kNR50), 0x77);
    EXPECT_EQ(apu.read(APU::kNR51), 0xf3);
}

TEST_F(APUTest, UnusedAddressesReadFF)
{
    powerOn();
    apu.write(0xff15, 0x00); // 0xff15 is unused
    apu.write(0xff1f, 0x00); // 0xff11 is unused
    EXPECT_EQ(apu.read(0xff15), 0xff);
    EXPECT_EQ(apu.read(0xff1f), 0xff);

    for (uint16_t a = 0xff27; a <= 0xff2f; ++a)
    {
        apu.write(a, 0x00);
        EXPECT_EQ(apu.read(a), 0xff) << std::hex << "addr=0x" << a;
    }
}

TEST_F(APUTest, AddressesOutsideApuRangeReadFF)
{
    EXPECT_EQ(apu.read(0xff0f), 0xff);
    EXPECT_EQ(apu.read(0xff00), 0xff);
    EXPECT_EQ(apu.read(0xff40), 0xff);
    EXPECT_NO_FATAL_FAILURE(apu.write(0xff00, 0x12)); // must not touch mRegs out of bounds
    EXPECT_NO_FATAL_FAILURE(apu.write(0xff0f, 0x12));
}

TEST_F(APUTest, Nr52InitialValueIsPowerOff)
{
    EXPECT_EQ(apu.read(APU::kNR52), 0x70);
}

TEST_F(APUTest, Nr52PowerOnAndOff)
{
    powerOn();
    EXPECT_EQ(apu.read(APU::kNR52), 0xf0);
    powerOff();
    EXPECT_EQ(apu.read(APU::kNR52), 0x70);
}

TEST_F(APUTest, Nr52LowBitsAreReadOnly)
{
    apu.write(APU::kNR52, 0xff);
    EXPECT_EQ(apu.read(APU::kNR52), 0xf0); // channel bits must not be settable
    apu.write(APU::kNR52, 0x0f);           // bit 7 = 0 -> power off
    EXPECT_EQ(apu.read(APU::kNR52), 0x70);
}

TEST_F(APUTest, Nr52WriteWhileOnWithBit7SetKeepsPower)
{
    powerOn();
    apu.write(APU::kNR52, 0x80);
    EXPECT_EQ(apu.read(APU::kNR52), 0xf0);
}

TEST_F(APUTest, WritesAreIgnoredWhilePoweredOff)
{
    for (auto addr : kRegisters)
    {
        apu.write(addr, 0xff);
    }

    powerOn();
    for (auto addr : kRegisters)
    {
        EXPECT_EQ(apu.read(addr), APU::kReadMask[addr - 0xff10]) << std::hex << "addr=0x" << addr;
    }
}

TEST_F(APUTest, PowerOffClearsRegisters)
{
    powerOn();
    apu.write(APU::kNR12, 0xf3);
    apu.write(APU::kNR22, 0xf3);
    apu.write(APU::kNR42, 0xf3);
    apu.write(APU::kNR50, 0x77);
    apu.write(APU::kNR51, 0xf3);

    powerOff();
    powerOn();

    EXPECT_EQ(apu.read(APU::kNR12), 0x00);
    EXPECT_EQ(apu.read(APU::kNR22), 0x00);
    EXPECT_EQ(apu.read(APU::kNR42), 0x00);
    EXPECT_EQ(apu.read(APU::kNR50), 0x00);
    EXPECT_EQ(apu.read(APU::kNR51), 0x00);
}

TEST_F(APUTest, PowerOffDoesNotClearPowerOnReRead)
{
    powerOn();
    powerOff();
    EXPECT_EQ(apu.read(APU::kNR52), 0x70);
    powerOn();
    EXPECT_EQ(apu.read(APU::kNR52), 0xf0);
}

TEST_F(APUTest, WaveRamReadWriteWhileOn)
{
    powerOn();
    for (uint16_t i = 0; i < 16; ++i)
    {
        apu.write(0xff30 + i, static_cast<uint8_t>(i * 0x11));
    }
    for (uint16_t i = 0; i < 16; ++i)
    {
        EXPECT_EQ(apu.read(0xff30 + i), static_cast<uint8_t>(i * 0x11));
    }
}

TEST_F(APUTest, WaveRamWritableWhileOff)
{
    apu.write(0xff30, 0xab);
    apu.write(0xff3f, 0xcd);
    EXPECT_EQ(apu.read(0xff30), 0xab);
    EXPECT_EQ(apu.read(0xff3f), 0xcd);
}

TEST_F(APUTest, WaveRamSurvivesPowerOff)
{
    powerOn();
    apu.write(0xff35, 0x99);
    powerOff();
    EXPECT_EQ(apu.read(0xff35), 0x99);
    powerOn();
    EXPECT_EQ(apu.read(0xff35), 0x99);
}

TEST_F(APUTest, LengthWritableWhilePoweredOff)
{
    apu.write(APU::kNR11, 0x00);
    apu.write(APU::kNR21, 0x3f);
    apu.write(APU::kNR31, 0x00);
    apu.write(APU::kNR41, 0x10);

    EXPECT_EQ(apu.getChannel(0).length, 64);
    EXPECT_EQ(apu.getChannel(1).length, 1);
    EXPECT_EQ(apu.getChannel(2).length, 256);
    EXPECT_EQ(apu.getChannel(3).length, 48);
}

TEST_F(APUTest, Ch3UsesAll8BitsForLength)
{
    apu.write(APU::kNR31, 0xff);
    EXPECT_EQ(apu.getChannel(2).length, 1);
    apu.write(APU::kNR31, 0x80);
    EXPECT_EQ(apu.getChannel(2).length, 128);
}

TEST_F(APUTest, LengthLoadedWhilePoweredOn)
{
    powerOn();
    apu.write(APU::kNR11, 0x10);
    apu.write(APU::kNR31, 0x10);
    EXPECT_EQ(apu.getChannel(0).length, 48);
    EXPECT_EQ(apu.getChannel(2).length, 240);
}

TEST_F(APUTest, LengthSurvivesPowerOff)
{
    powerOn();
    apu.write(APU::kNR11, 0x10);
    apu.write(APU::kNR41, 0x20);
    powerOff();
    EXPECT_EQ(apu.getChannel(0).length, 48);
    EXPECT_EQ(apu.getChannel(3).length, 32);
}

TEST_F(APUTest, PowerOffDisablesChannels)
{
    powerOn();
    powerOff();
    for (uint8_t i = 0; i < 4; ++i)
    {
        EXPECT_FALSE(apu.getChannel(i).enabled);
    }
}