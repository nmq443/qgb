#include "Emulator.h"
#include <iostream>

namespace qgb
{
Emulator::Emulator()
{
    mBus.init(mCartridge, mTimer, mPPU, mAPU);
    mCPU.init(mBus, mClock);
    mClock.addDevice(mTimer);
    mClock.addDevice(mPPU);
    mClock.addDevice(mAPU);
}

void Emulator::load(const std::filesystem::path &romPath)
{
    mCartridge.load(romPath);
}

void Emulator::step()
{
    mCPU.step();
    pollSerial();
}

void Emulator::run()
{
    while (mRunning)
    {
        step();
        pollSerial();
    }
}

void Emulator::stop()
{
    mRunning = false;
}

void Emulator::setSerialCallback(SerialCallback cb)
{
    mSerialCallback = std::move(cb);
}

void Emulator::pollSerial()
{
    if (mBus.read(0xff02) == 0x81)
    {
        char c = static_cast<char>(mBus.read(0xff01));
        mBus.write(0xFF02, 0x00);
        if (mSerialCallback)
        {
            mSerialCallback(c);
        }
    }
}

auto Emulator::getPPU() const -> const PPU &
{
    return mPPU;
}

auto Emulator::getPPU() -> PPU &
{
    return mPPU;
}

void Emulator::setJoypadState(JoypadState state)
{
    mBus.setJoypadState(state);
}

auto Emulator::hasRom() -> bool
{
    return mCartridge.hasRom();
}

auto Emulator::getAudioRingBuffer() -> AudioRing &
{
    return mAPU.getRingBuffer();
}
} // namespace qgb
