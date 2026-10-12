#pragma once

#include "Cartridge.h"
#include "PPU.h"
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

struct InterruptController
{
    uint8_t IF = 0xE1;
    uint8_t IE = 0x00;

    void request(Interrupt i);
    uint8_t pending() const;
    void clear(uint8_t bit);
};

class Bus
{
public:
    Bus() = default;
    void init(Cartridge &cartridge, Timer &timer, PPU &ppu);
    [[nodiscard]] uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
    const InterruptController &getInterruptController() const;
    InterruptController &getInterruptController();
    ~Bus();

private:
    void raiseInterrupt(Interrupt);

    Cartridge *mCartridgePtr = nullptr; // non-owning
    Timer *mTimerPtr = nullptr;         // non-owning
    PPU *mPPUPtr = nullptr;             // non-owning
    InterruptController mIC;
    std::array<uint8_t, 0x2000> mVram = {};
    std::array<uint8_t, 0x2000> mWram = {};
    std::array<uint8_t, 0x7f> mHram = {};
    std::array<uint8_t, 0xa0> mOam = {};    // placeholder
    std::array<uint8_t, 0x80> mIoRegs = {}; // placeholder
};
} // namespace qgb
