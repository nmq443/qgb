#include "Bus.h"

namespace qgb
{
Bus::Bus()
{
    auto timerInterrupt = [this]()
    {
        raiseInterrupt(Interrupt::Timer);
    };
    mTimer.setTimerInterrupt(timerInterrupt);
}

uint8_t Bus::read(uint16_t address) const
{
    if ((address <= 0x7fff) or (0xa000 <= address && address <= 0xbfff)) // rom bank
    {
        return mCartridge.read(address);
    }
    else if (0x8000 <= address && address <= 0x9fff) // video ram
    {
        return mVram[address - 0x8000];
    }
    else if (0xc000 <= address && address <= 0xdfff) // wram
    {
        return mWram[address - 0xc000];
    }
    else if (0xe000 <= address && address <= 0xfdff) // echo ram (mirror of 0xc000 - 0xddff)
    {
        return mWram[address - 0xe000];
    }
    else if (0xfe00 <= address && address <= 0xfe9f) // oam
    {
        return mOam[address - 0xfe00];
    }
    else if (0xfea0 <= address && address <= 0xfeff) // not usable
    {
        // intentionally ignored
        return 0;
    }
    else if (0xff00 <= address && address <= 0xff7f) // i/o registers
    {
        if (0xff0f == address)
        {
            return mIF;
        }
        else if (0xff04 <= address && address <= 0xff07)
        {
            return mTimer.read(address);
        }
        return mIoRegs[address - 0xff00];
    }
    else if (0xff80 <= address && address <= 0xfffe) // hram
    {
        return mHram[address - 0xff80];
    }
    else if (0xffff == address) // interrupt enable register
    {
        return mIE;
    }
    return 0;
}

void Bus::write(uint16_t address, uint8_t value)
{
    if ((address <= 0x7fff) or (0xa000 <= address && address <= 0xbfff)) // rom bank or external ram
    {
        mCartridge.write(address, value);
    }
    else if (0x8000 <= address && address <= 0x9fff) // video ram
    {
        mVram[address - 0x8000] = value;
    }
    else if (0xc000 <= address && address <= 0xdfff) // wram
    {
        mWram[address - 0xc000] = value;
    }
    else if (0xe000 <= address && address <= 0xfdff) // echo ram (mirror of 0xc000 - 0xddff)
    {
        mWram[address - 0xe000] = value;
    }
    else if (0xfe00 <= address && address <= 0xfe9f) // oam
    {
        mOam[address - 0xfe00] = value;
    }
    else if (0xfea0 <= address && address <= 0xfeff) // not usable
    {
        // intentionally ignored
    }
    else if (0xff00 <= address && address <= 0xff7f) // i/o registers
    {
        if (0xff0f == address) // interrupt flag
        {
            mIF = value;
        }
        else if (0xff04 <= address && address <= 0xff07)
        {
            mTimer.write(address, value);
        }
        else
        {
            mIoRegs[address - 0xff00] = value;
        }
    }
    else if (0xff80 <= address && address <= 0xfffe) // hram
    {
        mHram[address - 0xff80] = value;
    }
    else if (0xffff == address) // interrupt enable register
    {
        mIE = value;
    }
}

void Bus::loadCartridge(const std::filesystem::path &romPath)
{
    mCartridge.load(romPath);
}

void Bus::tick(int tCycles)
{
    mTimer.tick(tCycles);
}

void Bus::raiseInterrupt(Interrupt interrupt)
{
    mIF |= (1 << static_cast<int>(interrupt));
}
} // namespace qgb