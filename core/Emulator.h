#pragma once

#include "Bus.h"
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
    Bus mBus;
    bool mRunning = true;
};
} // namespace qgb
