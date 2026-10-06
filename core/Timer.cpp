#include "Timer.h"

#include <array>

namespace qgb
{
uint8_t Timer::read(uint16_t address) const
{
    switch (address)
    {
    case 0xff04:
        return mDiv;
    case 0xff05:
        return mTima;
    case 0xff06:
        return mTma;
    case 0xff07:
        return mTac;
    default:
        return 0;
    }
}

void Timer::write(uint16_t address, uint8_t value)
{
    switch (address)
    {
    case 0xff04:
        mDiv = 0;
        break;
    case 0xff05:
        mTima = value;
        break;
    case 0xff06:
        mTma = value;
        break;
    case 0xff07:
        mTac = value;
        break;
    default:
        break;
    }
}

void Timer::tick(int tCycles)
{
    mDivAccum += tCycles;
    mTimaAccum += tCycles;

    while (mDivAccum >= 256)
    {
        ++mDiv;
        mDivAccum -= 256;
    }

    bool enable = (mTac >> 2) & 0x1;
    static constexpr std::array thresholds = {256 * 4, 4 * 4, 16 * 4, 64 * 4};

    if (enable)
    {
        uint8_t clock = mTac & 0x3;

        while (mTimaAccum >= thresholds[clock])
        {
            if (mTima == 0xff)
            {
                mTima = mTma;
                mInterruptRequested = true;
                mTimerInterrupt();
            }
            else
            {
                ++mTima;
            }
            mTimaAccum -= thresholds[clock];
        }
    }
}

void Timer::setTimerInterrupt(const std::function<void()>& timerInterrupt)
{
    mTimerInterrupt = timerInterrupt;
}

bool Timer::interruptRequested() const
{
    return mInterruptRequested;
}
} // namespace qgb