#pragma once

#include "IClockable.h"
#include <array>
#include <cstdint>
#include <functional>

namespace qgb
{
enum class PPUMode : uint8_t
{
    HBlank,
    VBlank,
    OamScan,
    Drawing
};

enum class LCDC : uint8_t
{
    BgAndWindowEnable,
    ObjEnable,
    ObjSize,
    BgTilemap,
    BgAndWindowTiles,
    WindowEnable,
    WindowTilemap,
    LcdAndPPUEnable
};

enum class Stat : uint8_t
{
    PPUMode = 0,        // 2 bits wide (bits 0-1)
    LycEqualsLy = 2,    // bit 2
    Mode0Interrupt = 3, // bit 3
    Mode1Interrupt = 4, // bit 4
    Mode2Interrupt = 5, // bit 5
    LycInterrupt = 6    // bit 6
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
    static constexpr int kTileMapWidth = 32;      // tiles per row
    static constexpr int kTileMapWidthInPx = 256; // tilemap width in pixels
    static constexpr int kTileSize = 8;           // pixels per tile side
    static constexpr int kWindowXOffset = 7;

    PPU() = default;

    auto read(uint16_t address) const -> uint8_t;
    void write(uint16_t address, uint8_t value);
    void tick(int tCycles) override;
    auto getFrameBuffer() const -> const std::array<uint8_t, kLcdHeight * kLcdWidth> &;
    bool isFrameReady() const;
    void setFrameReady(bool frameReady);
    void setStatInterrupt(const std::function<void()> &statInterrupt);
    void setVBlankInterrupt(const std::function<void()> &vblankInterrupt);
    auto getTileDataAddress(int tileId) const -> uint16_t;
    auto getBgTilemapAddr() const -> uint16_t;
    auto getWindowTilemapAddr() const -> uint16_t;
    void setMode(PPUMode mode);
    auto getMode() const -> PPUMode;
    void renderScanline();
    void renderBackgroundScanline();
    void renderWindowScanline();
    void updateLyc();
    auto refreshLycFlag() -> bool;
    void updateModeWithinCurrentLine();
    auto fetchTileColorIndex(uint8_t tileId, int pixelX, int pixelY) const -> uint8_t;

private:
    bool mIsFrameReady{false};
    bool mLineRendered{false};
    uint8_t mLy = 0;
    uint8_t mLyc = 0;
    uint8_t mScx = 0;
    uint8_t mScy = 0;
    uint8_t mBgp = 0;
    uint8_t mObp0 = 0;
    uint8_t mObp1 = 0;
    uint8_t mDma = 0;
    uint8_t mWx = 0;
    uint8_t mWy = 0;
    uint8_t mLcdc = 0;
    int mDot = 0;
    int mWindowLine = 0;
    uint8_t mStat = 0;
    std::array<uint8_t, 0x2000> mVram = {};
    std::array<uint8_t, 0xa0> mOam = {};
    std::array<uint8_t, kLcdHeight * kLcdWidth> mFrameBuffer = {};
    PPUMode mMode = PPUMode::HBlank;
    std::function<void()> mStatInterrupt = nullptr;
    std::function<void()> mVBlankInterrupt = nullptr;
};
} // namespace qgb
