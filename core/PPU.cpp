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
        uint8_t lycFlag = (mLy == mLyc) ? 0x04 : 0x00;
        // bit 7 are always 1
        // bit 3-6: game writable
        // bit 2: lyc flag
        // bit 0-1: ppu mode
        return 0x80 | mStatEnable | lycFlag | static_cast<int>(mMode);
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
        mStatEnable = value & 0x78; // only bit 3-6 are writable
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
    mIsFrameReady = false;
    if (!(mLcdc & 0x80)) // LCD off, do nothing
    {
        return;
    }

    while (tCycles > 0)
    {
        tCycles--;

        ++mDot;
        if (mLy < kVBlankStart)
        {
            if (mDot == kOamScanDots)
            {
                setMode(PPUMode::Drawing);
            }
            else if (mDot == kOamScanDots + kDrawingDots)
            {
                renderScanline();
                setMode(PPUMode::HBlank);
                updateStatLine();
            }
        }

        if (mDot == kDotsPerLine)
        {
            mDot = 0;
            ++mLy;
            updateStatLine();

            if (mLy == kVBlankStart)
            {
                setMode(PPUMode::VBlank);
                if (mVBlankInterrupt)
                {
                    mVBlankInterrupt();
                }
                mIsFrameReady = true;
            }
            else if (mLy == kLinesPerFrame)
            {
                mLy = 0;
                setMode(PPUMode::OamScan);
            }
            else if (mLy < kVBlankStart)
            {
                setMode(PPUMode::OamScan);
            }
            // lines 145-153 simply stay in VBlank
        }
    }
}

void PPU::setMode(PPUMode mode)
{
    mMode = mode;
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

    uint16_t mapBase{getBgTilemapAddr()};             // background's tilemap start address
    uint8_t rowInTile{static_cast<uint8_t>(mLy % 8)}; // current row (0-7) in the tile

    for (int colInTile{}; colInTile < kLcdWidth; ++colInTile)
    {
        // Get tile coordinate on the screen based on current pixel (x, ly)
        uint8_t tileRow{static_cast<uint8_t>(mLy / 8)};
        uint8_t tileCol{static_cast<uint8_t>(colInTile / 8)};

        // Get tilemap address: base's address + tilemap offset
        // Since the tiles in tilemap is saved as a 1d array
        // We need to convert from 2d -> 1d coordinates
        // Using formula: index = row * WIDTH + col
        uint16_t mapAddr{static_cast<uint16_t>(mapBase + tileRow * kLcdWidth + tileCol)};

        // Each tile map contains the 1-byte indexes of the tiles to be displayed.
        // Not the data itself.
        uint8_t tileId{read(mapAddr)};

        // Based on the tile index, we get the tile address
        uint16_t tileAddr{getTileDataAddress(tileId)};

        // Create a tile structure from raw 16 bytes
        Tile tile{decodeTile(tileAddr)};

        // Then assign each pixel
        uint8_t colorIndex{tile[rowInTile][colInTile % 8]};
        mFrameBuffer[mLy * kLcdWidth + colInTile] = colorIndex;
    }
}

void PPU::updateStatLine()
{
    bool lycMatch = mLy == mLyc;
    bool line = (lycMatch && (mStatEnable & 0x40)) or                  // bit 6
                (mMode == PPUMode::OamScan && (mStatEnable & 0x20)) or // bit 5
                (mMode == PPUMode::VBlank && (mStatEnable & 0x10)) or  // bit 4
                (mMode == PPUMode::HBlank && (mStatEnable & 0x08));    // bit 3

    if (line and !mPrevStatline)
    {
        if (mStatInterrupt)
        {
            mStatInterrupt();
        }
    }
    mPrevStatline = line;
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
