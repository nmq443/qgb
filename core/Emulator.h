#pragma once

#include "CPU.h"

namespace qgb
{
class Emulator
{
public:
    using SerialCallback = std::function<void(char)>;

    Emulator();
    Emulator(const Emulator &) = delete;            // member hold references to each other
    Emulator &operator=(const Emulator &) = delete; // member hold references to each other

    void load(const std::filesystem::path &romPath);
    void run();
    void stop();
    void step();
    void setSerialCallback(SerialCallback cb);
    auto getPPU() const -> const PPU &;

private:
    void pollSerial();

    CPU mCPU;
    PPU mPPU;
    Timer mTimer;
    Cartridge mCartridge;
    Bus mBus;
    Clock mClock;
    SerialCallback mSerialCallback;
    bool mRunning = true;
};
} // namespace qgb
