#pragma once

#include "Cartridge.h"
#include "Timer.h"
#include <array>
#include <cstdint>

namespace qgb
{
enum class Interrupt
{
    VBlank,
    Lcd,
    Timer,
    Serial,
    Joypad
};

class Bus
{
public:
    Bus() = default;
    void init(Cartridge &cartridge, Timer &timer);
    [[nodiscard]] uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
    void raiseInterrupt(Interrupt);

    Cartridge *mCartridgePtr = nullptr; // non-owning
    Timer *mTimerPtr = nullptr;         // non-owning
    std::array<uint8_t, 0x2000> mVram = {};
    std::array<uint8_t, 0x2000> mWram = {};
    std::array<uint8_t, 0x7f> mHram = {};
    std::array<uint8_t, 0xa0> mOam = {};    // placeholder
    std::array<uint8_t, 0x80> mIoRegs = {}; // placeholder
    uint8_t mIE = 0;                        // interrupt enable
    uint8_t mIF = 0;                        // interrupt flag
};
} // namespace qgb
