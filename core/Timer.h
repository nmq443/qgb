#pragma once

#include <cstdint>
#include <functional>

namespace qgb
{
class Timer
{
public:
    Timer() = default;
    [[nodiscard]] uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
    void tick(int tCycles);
    void setTimerInterrupt(const std::function<void()>& timerInterrupt);
    bool interruptRequested() const;

private:
    uint8_t mDiv = 0;
    uint8_t mTima = 0;
    uint8_t mTma = 0;
    uint8_t mTac = 0;
    int mDivAccum = 0; // number of t-cycles accumulated for DIV register
    int mTimaAccum = 0; // number of t-cycles accumulated for TIMA register
    bool mInterruptRequested = false;
    std::function<void()> mTimerInterrupt = nullptr;
};
} // namespace qgb
