#include "PPU.h"
#include "Utils.h"

namespace qgb
{
auto PPU::read(uint16_t address) const -> uint8_t
{
    if (0x8000 <= address && address <= 0x9fff) // video ram
    {
        return mVram[address - 0x8000];
    }
    else if (0xfe00 <= address && address <= 0xfe9f) // oam
    {
        return mOam[address - 0xfe00];
    }
    else if (0xff44 == address)
    {
        return mLy;
    }
    else if (0xff45 == address)
    {
        return mLyc;
    }
    else if (0xff40 == address)
    {
        return mLcdc;
    }
    else if (0xff41 == address)
    {
        return mStat;
    }
    else if (0xff46 == address)
    {
        return mDma;
    }
    else if (0xff4a == address)
    {
        return mWy;
    }
    else if (0xff4b == address)
    {
        return mWx;
    }
    else if (0xff42 == address)
    {
        return mScy;
    }
    else if (0xff43 == address)
    {
        return mScx;
    }
    else if (0xff47 == address)
    {
        return mBgp;
    }
    else if (0xff48 == address)
    {
        return mObp0;
    }
    else if (0xff49 == address)
    {
        return mObp1;
    }
    return 0;
}

void PPU::write(uint16_t address, uint8_t value)
{
    if (0x8000 <= address && address <= 0x9fff) // video ram
    {
        mVram[address - 0x8000] = value;
    }
    else if (0xfe00 <= address && address <= 0xfe9f) // oam
    {
        mOam[address - 0xfe00] = value;
    }
    else if (0xff44 == address)
    {
        // mLy = value; ly is read only
    }
    else if (0xff45 == address)
    {
        mLyc = value;
        refreshLycFlag();
    }
    else if (0xff40 == address)
    {
        bool wasOn{(mLcdc & 0x80) > 0};
        mLcdc = value;
        bool isOn{(mLcdc & 0x80) > 0};

        if (wasOn and !isOn)
        {
            // lcd turn off
            mLy = 0;
            mDot = 0;
            setMode(PPUMode::HBlank);
            std::fill(std::begin(mFrameBuffer), std::end(mFrameBuffer), 0);
        }
        else if (!wasOn and isOn)
        {
            // lcd turn on: start fresh at line 0
            mLy = 0;
            mDot = 0;
            setMode(PPUMode::OamScan);
        }
    }
    else if (0xff41 == address)
    {
        mStat = value & 0b1111'1000;
    }
    else if (0xff46 == address)
    {
        mDma = value;
    }
    else if (0xff4a == address)
    {
        mWy = value;
    }
    else if (0xff4b == address)
    {
        mWx = value;
    }
    else if (0xff42 == address)
    {
        mScy = value;
    }
    else if (0xff43 == address)
    {
        mScx = value;
    }
    else if (0xff47 == address)
    {
        mBgp = value;
    }
    else if (0xff48 == address)
    {
        mObp0 = value;
    }
    else if (0xff49 == address)
    {
        mObp1 = value;
    }
}

auto PPU::getFrameBuffer() const -> const std::array<uint8_t, kLcdHeight * kLcdWidth> &
{
    return mFrameBuffer;
}

void PPU::tick(int tCycles)
{
    if (!getBit(mLcdc, static_cast<uint8_t>(LCDC::LcdAndPPUEnable))) // LCD off, do nothing
    {
        mLy = 0;
        mDot = 0;
        setMode(PPUMode::HBlank);
        refreshLycFlag();
        return;
    }

    updateModeWithinCurrentLine();
    mDot += tCycles;
    updateModeWithinCurrentLine();

    while (mDot >= kDotsPerLine)
    {
        mDot -= kDotsPerLine;

        if (mLy < kLcdHeight)
        {
            renderScanline();
        }

        ++mLy;

        if (kLcdHeight == mLy)
        {
            if (mVBlankInterrupt)
            {
                mVBlankInterrupt();
            }
            mIsFrameReady = true;
        }
        else if (mLy >= kLinesPerFrame)
        {
            mLy = 0;
        }

        updateLyc();
        updateModeWithinCurrentLine();
    }
}

void PPU::setMode(PPUMode mode)
{
    mMode = mode;
    mStat &= 0b1111'1100; // clear bit 0 and 1 to set new mode
    mStat |= static_cast<uint8_t>(mode);
}

auto PPU::getMode() const -> PPUMode
{
    return static_cast<PPUMode>(mStat & 0b0000'0011);
}

void PPU::renderScanline()
{
    if (!getBit(mLcdc, static_cast<uint8_t>(LCDC::BgAndWindowEnable)))
    {
        for (int x{}; x < kLcdWidth; ++x)
        {
            mFrameBuffer[mLy * kLcdWidth + x] = 0;
        }
        return;
    }

    uint16_t mapBase{getBgTilemapAddr()}; // background's tilemap start address

    for (int x{}; x < kLcdWidth; ++x)
    {
        // Transform from screen coordinate to tilemap coordinate
        uint8_t mapX{static_cast<uint8_t>(x + mScx)};
        uint8_t mapY{static_cast<uint8_t>(mLy + mScy)};

        // Get tile coordinate on the screen based on current pixel (x, ly)
        uint8_t tileRow{static_cast<uint8_t>(mapY / 8)};
        uint8_t tileCol{static_cast<uint8_t>(mapX / 8)};

        // Get tilemap address: base's address + tilemap offset
        // Since the tiles in tilemap is saved as a 1d array
        // We need to convert from 2d -> 1d coordinates
        // Using formula: index = row * WIDTH + col
        uint16_t mapAddr{static_cast<uint16_t>(mapBase + tileRow * 32 + tileCol)}; // 1 tile is 32 bytes in size

        // Each tile map contains the 1-byte indexes of the tiles to be displayed.
        // Not the data itself.
        uint8_t tileId{read(mapAddr)};

        // Based on the tile index, we get the tile address
        uint16_t tileAddr{getTileDataAddress(tileId)};

        // Create a tile structure from raw 16 bytes
        Tile tile{decodeTile(tileAddr)};

        // Then assign each pixel
        uint8_t pixelX{static_cast<uint8_t>(mapX % 8)};
        uint8_t pixelY{static_cast<uint8_t>(mapY % 8)};
        uint8_t colorIndex{tile[pixelY][pixelX]};
        uint8_t shade{static_cast<uint8_t>((mBgp >> (colorIndex * 2)) & 0x03)};
        mFrameBuffer[mLy * kLcdWidth + x] = shade;
    }
}

void PPU::updateLyc()
{
    bool oldCoincidence{getBit(mStat, static_cast<uint8_t>(Stat::LycEqualsLy))};
    bool newCoincidence{refreshLycFlag()};
    if (!oldCoincidence and newCoincidence and getBit(mStat, static_cast<uint8_t>(Stat::LycInterrupt)))
    {
        if (mStatInterrupt)
        {
            mStatInterrupt();
        }
    }
}

auto PPU::refreshLycFlag() -> bool
{
    bool status{mLy == mLyc};
    mStat = setBit(mStat, static_cast<uint8_t>(Stat::LycEqualsLy), status);
    return status;
}

void PPU::updateModeWithinCurrentLine()
{
    bool ret{false};

    PPUMode oldMode{getMode()};
    if (mLy >= kLcdHeight)
    {
        setMode(PPUMode::VBlank);
    }
    else if (mDot < kOamScanDots)
    {
        setMode(PPUMode::OamScan);
    }
    else if (mDot < kOamScanDots + kDrawingDots)
    {
        setMode(PPUMode::Drawing);
    }
    else
    {
        setMode(PPUMode::HBlank);
    }

    PPUMode currentMode{getMode()};
    if (currentMode != oldMode)
    {
        ret |= PPUMode::OamScan == currentMode and getBit(mStat, static_cast<uint8_t>(Stat::Mode2Interrupt));
        ret |= PPUMode::VBlank == currentMode and getBit(mStat, static_cast<uint8_t>(Stat::Mode1Interrupt));
        ret |= PPUMode::HBlank == currentMode and getBit(mStat, static_cast<uint8_t>(Stat::Mode0Interrupt));
    }
    if (ret and mStatInterrupt)
    {
        mStatInterrupt();
    }
}

bool PPU::isFrameReady() const
{
    return mIsFrameReady;
}

void PPU::setFrameReady(bool frameReady)
{
    mIsFrameReady = frameReady;
}

void PPU::setStatInterrupt(const std::function<void()> &statInterrupt)
{
    mStatInterrupt = statInterrupt;
}

void PPU::setVBlankInterrupt(const std::function<void()> &vblankInterrupt)
{
    mVBlankInterrupt = vblankInterrupt;
}

auto PPU::decodeTile(uint16_t tileAddr) const -> Tile
{
    Tile tile{};

    for (int row{}; row < 8; ++row)
    {
        uint16_t rowAddress = tileAddr + (row * 2);
        uint8_t lo = read(rowAddress);
        uint8_t hi = read(rowAddress + 1);
        for (int col{}; col < 8; ++col)
        {
            int bit{7 - col}; // current bit index
            int higherBitOfCurrentPixel{(hi >> bit) & 1};
            int lowerBitOfCurrentPixel{(lo >> bit) & 1};
            tile[row][col] = (higherBitOfCurrentPixel << 1) | lowerBitOfCurrentPixel;
        }
    }

    return tile;
}

auto PPU::getTileDataAddress(int tileId) const -> uint16_t
{
    uint16_t address{};                                              // tile address = based address + index * sizeof(a signle tile which is 16 bytes)
    if (getBit(mLcdc, static_cast<uint8_t>(LCDC::BgAndWindowTiles))) // 0x8000 - 0x8fff
    {
        // unsigned
        return 0x8000 + tileId * 16; // 1 tile is 16 bytes
    }
    else // 0x8800 - 0x97ff
    {
        // signed
        return 0x9000 + static_cast<int8_t>(tileId) * 16; // 1 tile is 16 bytes
    }
    return address;
}

auto PPU::getBgTilemapAddr() const -> uint16_t
{
    // lcdc bit 3 = 0 then 0x9800-0x9BFF, otherwise 0x9C00-0x9FFF
    return getBit(mLcdc, static_cast<uint8_t>(LCDC::BgTilemap)) ? 0x9c00 : 0x9800;
}
} // namespace qgb
