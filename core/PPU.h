#pragma once

#include "IClockable.h"
#include <array>
#include <cstdint>

namespace qgb
{
class PPU : public IClockable
{
public:
    static constexpr int kLcdHeight = 144;
    static constexpr int kLcdWidth = 160;

    PPU() = default;

    auto read(uint16_t address) const -> uint8_t;
    void write(uint16_t address, uint8_t value);
    void tick(int tCycles) override;
    auto getFrameBuffer() const -> const std::array<uint8_t, kLcdHeight * kLcdWidth> &;
    bool isFrameReady() const;
    void setFrameReady(bool frameReady);

private:
    bool mIsFrameReady = true;
    uint8_t mLy = 0;
    uint8_t mLyc = 0;
    uint8_t mStat = 0;
    std::array<uint8_t, 0x2000> mVram = {};
    std::array<uint8_t, 0xa0> mOam = {};
    std::array<uint8_t, kLcdHeight * kLcdWidth> mFrameBuffer = {};
};
} // namespace qgb
