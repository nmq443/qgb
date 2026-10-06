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
        int mCycles = mCPU.step(mBus);
        mTimer.tick(mCycles * 4);

        if (mBus.read(0xff02) == 0x81)
        {
            char c = static_cast<char>(mBus.read(0xff01));

            mBus.write(0xFF02, 0x00);

            std::cout << c << std::flush;
        }
    }
}
} // namespace qgb