#include "Emulator.h"
#include <iostream>

namespace qgb
{
Emulator::Emulator()
{
    mBus.init(mCartridge, mTimer);
}

void Emulator::load(const std::filesystem::path &romPath)
{
    mCartridge.load(romPath);
}

void Emulator::run()
{
    while (mRunning)
    {
        int cycles = mCPU.step(mBus);
        mTimer.tick(cycles);
    }
}
} // namespace qgb