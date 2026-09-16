#pragma once

#include "CPU.h"

namespace qgb
{
    class Emulator
    {
    public:
        Emulator() = default;
        void run();
    private:
        CPU mCPU;
        bool mRunning = true;
    };
}

