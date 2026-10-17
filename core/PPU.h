#pragma once

#include "IClockable.h"
#include <array>
#include <cstdint>
#include <functional>

namespace qgb
{
enum class PPUMode
{
    HBlank,
    VBlank,
    OamScan,
    Drawing
};

using Tile = std::array<std::array<uint8_t, 8>, 8>;

class PPU : public IClockable
{
public:
    static constexpr int kLcdHeight = 144;
    static constexpr int kLcdWidth = 160;
    static constexpr int kDotsPerLine = 456;
    static constexpr int kLinesPerFrame = 154;
    static constexpr int kVBlankStart = 144;
    static constexpr int kOamScanDots = 80;
    static constexpr int kDrawingDots = 172;

    PPU() = default;

    auto read(uint16_t address) const -> uint8_t;
    void write(uint16_t address, uint8_t value);
    void tick(int tCycles) override;
    auto getFrameBuffer() const -> const std::array<uint8_t, kLcdHeight * kLcdWidth> &;
    bool isFrameReady() const;
    void setFrameReady(bool frameReady);
    void setStatInterrupt(const std::function<void()> &statInterrupt);
    void setVBlankInterrupt(const std::function<void()> &vblankInterrupt);

private:
    void setMode(PPUMode mode);
    void renderScanline();
    void updateStatLine();

    static Tile decodeTile(const uint8_t *data);

    bool mIsFrameReady{false};
    bool mPrevStatline{false};
    uint8_t mLy = 0;
    uint8_t mLyc = 0;
    uint8_t mScx{};
    uint8_t mScy{};
    uint8_t mBgp{};
    uint8_t mObp0{};
    uint8_t mObp1{};
    uint8_t mDma{};
    uint8_t mWx{};
    uint8_t mWy{};
    uint8_t mLcdc{};
    int mDot{};
    uint8_t mStatEnable{};
    std::array<uint8_t, 0x2000> mVram = {};
    std::array<uint8_t, 0xa0> mOam = {};
    std::array<uint8_t, kLcdHeight * kLcdWidth> mFrameBuffer = {};
    PPUMode mMode = PPUMode::HBlank;
    std::function<void()> mStatInterrupt = nullptr;
    std::function<void()> mVBlankInterrupt = nullptr;
};
} // namespace qgb
