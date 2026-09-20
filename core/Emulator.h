#pragma once

#include "CPU.h"
#include "Bus.h"

namespace qgb
{
    class Emulator
    {
    public:
        Emulator() = default;
        void run();
    private:
        CPU mCPU;
        Bus mBus;
        bool mRunning = true;
    };
}

