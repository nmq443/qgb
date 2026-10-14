#include "PPU.h"

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
            mStatEnable = 0;
            setMode(PPUMode::HBlank);
            std::fill(std::begin(mFrameBuffer), std::end(mFrameBuffer), 0);
        }
        else if (!wasOn and isOn)
        {
            // lcd turn on: start fresh at line 0
            mLy = 0;
            mDot = 0;
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
            }
        }

        if (mDot == kDotsPerLine)
        {
            mDot = 0;
            ++mLy;

            if (mLy == kVBlankStart)
            {
                setMode(PPUMode::VBlank);
                // TODO: request VBlank interrupt, signal "frame ready"
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
    // TODO: fill row ly of mRenderBuffer
}

void PPU::updateStatLine()
{
    bool lycMatch = mLy == mLyc;
    bool line = (lycMatch && (mStatEnable & 0x40)) or                  // bit 6
                (mMode == PPUMode::OamScan && (mStatEnable & 0x20)) or // bit 5
                (mMode == PPUMode::VBlank && (mStatEnable & 0x10)) or  // bit 4
                (mMode == PPUMode::HBlank && (mStatEnable & 0x80));    // bit 3

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
} // namespace qgb
