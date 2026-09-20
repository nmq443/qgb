#include "Emulator.h"
#include <iostream>

namespace qgb
{
    void Emulator::run()
    {
        while (mRunning)
        {
            mCPU.step(mBus);
        }
    }
}