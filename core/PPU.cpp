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
    if (0xfe00 <= address && address <= 0xfe9f) // oam
    {
        return mOam[address - 0xfe00];
    }

    switch (address)
    {
    case kLcdc:
        return mLcdc;
    case kStat:
        return mStat;
    case kScy:
        return mScy;
    case kScx:
        return mScx;
    case kLy:
        return mLy;
    case kLyc:
        return mLyc;
    case kDma:
        return mDma;
    case kBgp:
        return mBgp;
    case kObp0:
        return mObp0;
    case kObp1:
        return mObp1;
    case kWy:
        return mWy;
    case kWx:
        return mWx;
    default:
        return 0;
    }
}

void PPU::write(uint16_t address, uint8_t value)
{
    if (0x8000 <= address && address <= 0x9fff) // video ram
    {
        mVram[address - 0x8000] = value;
        return;
    }
    if (0xfe00 <= address && address <= 0xfe9f) // oam
    {
        mOam[address - 0xfe00] = value;
        return;
    }

    switch (address)
    {
    case kLy:
        // mLy = value; ly is read only
        break;
    case kLyc:
        mLyc = value;
        updateLyc();
        break;
    case kLcdc:
    {
        bool wasOn = (mLcdc & 0x80) > 0;
        mLcdc = value;
        bool isOn = (mLcdc & 0x80) > 0;

        if (wasOn and !isOn)
        {
            // lcd turn off
            mLy = 0;
            mWindowLine = 0;
            mDot = 0;
            mLineRendered = false;
            setMode(PPUMode::HBlank);
            mFrameBuffer.fill(0);
        }
        else if (!wasOn and isOn)
        {
            // lcd turn on: start fresh at line 0
            mLy = 0;
            mDot = 0;
            setMode(PPUMode::OamScan);
        }
        break;
    }
    case kStat:
        mStat = (mStat & 0b0000'0111) | (value & 0b1111'1000);
        break;
    case kDma:
        mDma = value;
        break;
    case kWy:
        mWy = value;
        break;
    case kWx:
        mWx = value;
        break;
    case kScy:
        mScy = value;
        break;
    case kScx:
        mScx = value;
        break;
    case kBgp:
        mBgp = value;
        break;
    case kObp0:
        mObp0 = value;
        break;
    case kObp1:
        mObp1 = value;
        break;
    default:
        break;
    }
}

auto PPU::getFrameBuffer() const -> const std::array<uint8_t, kLcdHeight * kLcdWidth> &
{
    return mFrameBuffer;
}

void PPU::tick(int tCycles)
{
    if (!getBit(mLcdc, LCDC::LcdAndPPUEnable)) // LCD off, do nothing
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
        mLineRendered = false; // new line begins
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
            mWindowLine = 0;
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

void PPU::renderBackgroundScanline()
{
    if (!getBit(mLcdc, LCDC::BgAndWindowEnable))
    {
        for (int x = 0; x < kLcdWidth; ++x)
        {
            mFrameBuffer[mLy * kLcdWidth + x] = 0;
        }
        mBgColorIndex.fill(0);
        return;
    }

    uint16_t mapBase = getBgTilemapAddr(); // background's tilemap start address
    const uint8_t bgY = (mLy + mScy) % kTileMapWidthInPx;
    const uint8_t tileMapRow = bgY / kTileSize;
    const uint8_t pixelY = bgY % kTileSize;

    for (int x = 0; x < kLcdWidth; ++x)
    {
        // Transform from screen coordinate to tilemap coordinate
        uint8_t bgX = (x + mScx) % kTileMapWidthInPx;

        // Get tile coordinate in the BG map
        uint8_t tileMapCol = bgX / kTileSize;

        // Get tilemap address: base's address + tilemap offset
        // Since the tiles in tilemap is saved as a 1d array
        // We need to convert from 2d -> 1d coordinates
        // Using formula: index = row * WIDTH + col
        uint16_t mapAddr = mapBase + tileMapRow * kTileMapWidth + tileMapCol; // tilemap has 32x32 tiles

        // Each tile map contains the 1-byte indexes of the tiles to be displayed.
        // Not the data itself.
        uint8_t tileId = read(mapAddr);

        // Current pixel coordinate on the tile
        uint8_t pixelX = bgX % kTileSize;

        auto colorIndex = fetchTileColorIndex(tileId, pixelX, pixelY);
        mBgColorIndex[x] = colorIndex;
        uint8_t shade = static_cast<uint8_t>((mBgp >> (colorIndex * 2)) & 0x03);
        mFrameBuffer[mLy * kLcdWidth + x] = shade;
    }
}

void PPU::renderWindowScanline()
{
    if (!getBit(mLcdc, LCDC::WindowEnable) or
        !getBit(mLcdc, LCDC::BgAndWindowEnable))
    {
        return;
    }

    if (mLy < mWy) // not reach the window's top yet
    {
        return;
    }

    int winX = mWx - kWindowXOffset; // screen x where window starts
    if (winX >= kLcdWidth)           // window is off-screen to the right
    {
        return;
    }

    uint16_t tileMapBase = getWindowTilemapAddr();
    for (int x = std::max(winX, 0); x < kLcdWidth; ++x)
    {
        int windowPixelX = x - winX;    // x inside the window
        int windowPixelY = mWindowLine; // internal window line counter

        int tileRow = windowPixelY / kTileSize; // row of the tile in window tilemap
        int tileCol = windowPixelX / kTileSize; // column of the tile in window tilemap

        uint16_t mapAddr = tileMapBase + tileRow * kTileMapWidth + tileCol;
        uint8_t tileId = read(mapAddr);
        int pixelX = windowPixelX % kTileSize;
        int pixelY = windowPixelY % kTileSize;
        auto colorIndex = fetchTileColorIndex(tileId, pixelX, pixelY);
        mBgColorIndex[x] = colorIndex;
        uint8_t shade = static_cast<uint8_t>((mBgp >> (colorIndex * 2)) & 0x03);
        mFrameBuffer[mLy * kLcdWidth + x] = shade;
    }
    ++mWindowLine;
}

void PPU::renderOamScanline()
{
    if (!getBit(mLcdc, LCDC::ObjEnable))
    {
        return;
    }

    auto sprites = scanOam();

    std::stable_sort(sprites.begin(), sprites.end(),
                     [](const Sprite &a, const Sprite &b)
                     { return a.x < b.x; });

    for (auto it = sprites.rbegin(); it != sprites.rend(); ++it)
    {
        drawSpriteRow(*it);
    }
}

void PPU::renderScanline()
{
    renderBackgroundScanline();
    renderWindowScanline();
    renderOamScanline();
}

void PPU::updateLyc()
{
    bool oldCoincidence = getBit(mStat, Stat::LycEqualsLy);
    bool newCoincidence = refreshLycFlag();
    if (!oldCoincidence and newCoincidence and getBit(mStat, Stat::LycInterrupt))
    {
        if (mStatInterrupt)
        {
            mStatInterrupt();
        }
    }
}

auto PPU::refreshLycFlag() -> bool
{
    bool status = (mLy == mLyc);
    mStat = setBit(mStat, Stat::LycEqualsLy, status);
    return status;
}

void PPU::updateModeWithinCurrentLine()
{
    bool ret = false;

    PPUMode oldMode = getMode();
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

    if (mLy < kLcdHeight and mDot >= kOamScanDots + kDrawingDots and !mLineRendered)
    {
        renderScanline();
        mLineRendered = true;
    }

    PPUMode currentMode = getMode();
    if (currentMode != oldMode)
    {
        ret |= PPUMode::OamScan == currentMode and getBit(mStat, Stat::Mode2Interrupt);
        ret |= PPUMode::VBlank == currentMode and getBit(mStat, Stat::Mode1Interrupt);
        ret |= PPUMode::HBlank == currentMode and getBit(mStat, Stat::Mode0Interrupt);
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

auto PPU::getTileDataAddress(int tileId) const -> uint16_t
{
    if (getBit(mLcdc, LCDC::BgAndWindowTiles)) // 0x8000 - 0x8fff
    {
        // unsigned
        return 0x8000 + tileId * 16; // 1 tile is 16 bytes
    }
    else // 0x8800 - 0x97ff
    {
        // signed
        return 0x9000 + static_cast<int8_t>(tileId) * 16; // 1 tile is 16 bytes
    }
}

auto PPU::getBgTilemapAddr() const -> uint16_t
{
    // lcdc bit 3 = 0 then 0x9800-0x9BFF, otherwise 0x9C00-0x9FFF
    return getBit(mLcdc, LCDC::BgTilemap) ? 0x9c00 : 0x9800;
}

auto PPU::getWindowTilemapAddr() const -> uint16_t
{
    // lcdc bit 6 = 0 then 0x9800-0x9BFF, otherwise 0x9C00-0x9FFF
    return getBit(mLcdc, LCDC::WindowTilemap) ? 0x9c00 : 0x9800;
}

auto PPU::fetchTileColorIndex(uint8_t tileId, int pixelX, int pixelY) const -> uint8_t
{
    const uint16_t rowAddr = getTileDataAddress(tileId) + pixelY * 2;
    const uint8_t lo = read(rowAddr);
    const uint8_t hi = read(rowAddr + 1);
    const int bit = 7 - pixelX;
    return (((hi >> bit) & 1) << 1) | ((lo >> bit) & 1);
}

auto PPU::scanOam() const -> std::vector<Sprite>
{
    std::vector<Sprite> visible;
    int height = getBit(mLcdc, LCDC::ObjSize) ? 16 : 8;

    for (int i = 0; i < kMaxOamEntries && visible.size() < 10; ++i)
    {
        int base = i * 4;
        int top = mOam[base] - kObjVerticalOffset; // Y = Object's vertical position on the screen + 16
        if (mLy >= top and mLy < top + height)
        {
            visible.push_back({
                mOam[base],     // y
                mOam[base + 1], // x
                mOam[base + 2], // tileId
                mOam[base + 3]  // attributes
            });
        }
    }

    return visible;
}

void PPU::drawSpriteRow(const Sprite &sprite)
{
    int height = getBit(mLcdc, LCDC::ObjSize) ? kObjectMaxHeight : kObjectMinHeight;
    int top = sprite.y - kObjVerticalOffset;
    int left = sprite.x - kObjHorizontalOffset;

    int row = mLy - top; // 0..16
    if (getBit(sprite.flags, SpriteFlag::YFlip))
    {
        row = height - 1 - row; // flip over full height
    }

    int tileId = sprite.tileId;
    if (kObjectMaxHeight == height)
    {
        tileId &= 0xfe; // top tile
        if (row >= kTileSize)
        {
            tileId |= 0x01; // bottom tile
        }
    }
    int tileRow = row % kTileSize; // row inside chosen line

    uint16_t addr = 0x8000 + tileId * kTileSizeInBytes + tileRow * 2;
    uint8_t lo = read(addr);
    uint8_t hi = read(addr + 1);
    uint8_t palette = getBit(sprite.flags, SpriteFlag::DMGPalette) ? mObp1 : mObp0;

    for (int col = 0; col < kTileSize; ++col)
    {
        int bit = getBit(sprite.flags, SpriteFlag::XFlip) ? col : 7 - col;
        uint8_t colorIndex = (((hi >> bit) & 1) << 1) | ((lo >> bit) & 1);
        if (colorIndex == 0)
        {
            continue;
        }
        int x = left + col;
        if (x < 0 or x >= kLcdWidth) // off-screen
        {
            continue;
        }

        if (!getBit(sprite.flags, SpriteFlag::Priority) or mBgColorIndex[x] == 0)
        {
            mFrameBuffer[mLy * kLcdWidth + x] = (palette >> (colorIndex * 2)) & 3;
        }
    }
}

void PPU::setOam(std::span<const uint8_t, 160> oam)
{
    std::ranges::copy(oam, std::begin(mOam));
}
} // namespace qgb