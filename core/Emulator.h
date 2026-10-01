#pragma once

#include "Bus.h"
#include "CPU.h"

namespace qgb
{
class Emulator
{
public:
    Emulator();
    void load(const std::filesystem::path &romPath);
    void run();

private:
    CPU mCPU;
    Timer mTimer;
    Cartridge mCartridge;
    Bus mBus;

    bool mRunning = true;
};
} // namespace qgb
