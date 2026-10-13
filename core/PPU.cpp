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
        mLy = value;
    }
    else if (0xff45 == address)
    {
        mLyc = value;
    }
    else if (0xff41 == address)
    {
        mStat = value;
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
    mDot += tCycles;
    while (mDot > kDotsPerLine)
    {
        mDot -= kDotsPerLine;
        mLy = (mLy + 1) % kLinesPerFrame;
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
} // namespace qgb
