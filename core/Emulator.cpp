#include "Emulator.h"
#include <iostream>

namespace qgb
{
Emulator::Emulator()
{
    mBus.init(mCartridge, mTimer);
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