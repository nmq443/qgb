#include "core/Timer.h"
#include "core/Bus.h"
#include <gtest/gtest.h>

using namespace qgb;

TEST(TimerTests, InitialRegistersReadAsZero)
{
    Timer timer;

    EXPECT_EQ(timer.read(0xFF04), 0x00); // DIV
    EXPECT_EQ(timer.read(0xFF05), 0x00); // TIMA
    EXPECT_EQ(timer.read(0xFF06), 0x00); // TMA
    EXPECT_EQ(timer.read(0xFF07), 0x00); // TAC
}

TEST(TimerTests, WriteAndReadTIMA)
{
    Timer timer;

    timer.write(0xFF05, 0xAB);

    EXPECT_EQ(timer.read(0xFF05), 0xAB);
}

TEST(TimerTests, WriteAndReadTMA)
{
    Timer timer;

    timer.write(0xFF06, 0xCD);

    EXPECT_EQ(timer.read(0xFF06), 0xCD);
}

TEST(TimerTests, WriteAndReadTAC)
{
    Timer timer;

    timer.write(0xFF07, 0x05);

    EXPECT_EQ(timer.read(0xFF07), 0x05);
}

TEST(TimerTests, WritingDIVResetsDIVToZero)
{
    Timer timer;

    timer.tick(
        64 * 4); // enough to increment DIV if your DIV increments every 64 m-cycles

    EXPECT_NE(timer.read(0xFF04), 0x00);

    timer.write(0xFF04, 0x99);

    EXPECT_EQ(timer.read(0xFF04), 0x00);
}

TEST(TimerTests, DIVIncrementsEvery64MCycles)
{
    Timer timer;

    timer.tick(63 * 4);
    EXPECT_EQ(timer.read(0xFF04), 0x00);

    timer.tick(1 * 4);
    EXPECT_EQ(timer.read(0xFF04), 0x01);

    timer.tick(64 * 4);
    EXPECT_EQ(timer.read(0xFF04), 0x02);
}

TEST(TimerTests, TIMADoesNotIncrementWhenTimerDisabled)
{
    Timer timer;

    timer.write(0xFF07, 0x00); // timer disabled

    timer.tick(1024 * 4);

    EXPECT_EQ(timer.read(0xFF05), 0x00);
}

TEST(TimerTests, TIMAIncrementsAt4096Hz)
{
    Timer timer;

    timer.write(0xFF07, 0x04); // enable, frequency select 00

    timer.tick(255 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x00);

    timer.tick(1 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x01);
}

TEST(TimerTests, TIMAIncrementsAt262144Hz)
{
    Timer timer;

    timer.write(0xFF07, 0x05); // enable, frequency select 01

    timer.tick(3 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x00);

    timer.tick(1 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x01);
}

TEST(TimerTests, TIMAIncrementsAt65536Hz)
{
    Timer timer;

    timer.write(0xFF07, 0x06); // enable, frequency select 10

    timer.tick(15 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x00);

    timer.tick(1 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x01);
}

TEST(TimerTests, TIMAIncrementsAt16384Hz)
{
    Timer timer;

    timer.write(0xFF07, 0x07); // enable, frequency select 11

    timer.tick(63 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x00);

    timer.tick(1 * 4);
    EXPECT_EQ(timer.read(0xFF05), 0x01);
}

TEST(TimerTests, TIMAOverflowReloadsFromTMAAndRequestsInterrupt)
{
    Cartridge cartridge;
    Timer timer;
    Bus bus;
    bus.init(cartridge, timer); // to set request interrupt callback

    timer.write(0xFF05, 0xFF); // TIMA
    timer.write(0xFF06, 0x42); // TMA
    timer.write(0xFF07, 0x05); // enable, fast frequency, 4 m-cycles

    timer.tick(4 * 4);
    bool interruptRequested = timer.interruptRequested();

    EXPECT_TRUE(interruptRequested);
    EXPECT_EQ(timer.read(0xFF05), 0x42);
}

TEST(TimerTests, TIMANormalIncrementDoesNotRequestInterrupt)
{
    Timer timer;

    timer.write(0xFF05, 0x10);
    timer.write(0xFF07, 0x05); // enable, 4 m-cycles

    timer.tick(4 * 4);
    bool interruptRequested = timer.interruptRequested();

    EXPECT_FALSE(interruptRequested);
    EXPECT_EQ(timer.read(0xFF05), 0x11);
}

TEST(TimerTests, TickCanHandleMultipleIncrements)
{
    Timer timer;

    timer.write(0xFF05, 0x00);
    timer.write(0xFF07, 0x05); // enable, 4 m-cycles

    timer.tick(20 * 4);
    bool interruptRequested = timer.interruptRequested();

    EXPECT_FALSE(interruptRequested);
    EXPECT_EQ(timer.read(0xFF05), 0x05);
}