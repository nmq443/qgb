#include "Bus.h"

namespace qgb
{
    uint8_t Bus::read(uint16_t address) const
    {
        if (0x0000 <= address && address <= 0x3fff) // rom bank
        {

        }
        else if (0x4000 <= address && address <= 0x7fff)  // rom bank
        {

        }
        else if (0x8000 <= address && address <= 0x9fff) // video ram
        {

        }
        else if (0xa000 <= address && address <= 0xbfff) // external ram
        {

        }
        else if (0xc000 <= address && address <= 0xcfff) // wram
        {

        }
        else if (0xd000 <= address && address <= 0xdfff) // wram
        {

        }
        else if (0xe000 <= address && address <= 0xfdff) // echo ram (mirror of 0xc000 - 0xddff)
        {

        }
        else if (0xfe00 <= address && address <= 0xfe9f) // oam
        {

        }
        else if (0xfea0 <= address && address <= 0xfeff) // not usable
        {

        }
        else if (0xff00 <= address && address <= 0xff7f) // i/o registers
        {

        }
        else if (0xff80 <= address && address <= 0xfffe) // hram
        {

        }
        else if (0xffff == address) // interrupt enable register
        {

        }
        return 0;
    }

    void Bus::write(uint16_t address, uint8_t value)
    {
        if (0x0000 <= address && address <= 0x3fff) // rom bank
        {

        }
        else if (0x4000 <= address && address <= 0x7fff)  // rom bank
        {

        }
        else if (0x8000 <= address && address <= 0x9fff) // video ram
        {

        }
        else if (0xa000 <= address && address <= 0xbfff) // external ram
        {

        }
        else if (0xc000 <= address && address <= 0xcfff) // wram
        {

        }
        else if (0xd000 <= address && address <= 0xdfff) // wram
        {

        }
        else if (0xe000 <= address && address <= 0xfdff) // echo ram (mirror of 0xc000 - 0xddff)
        {

        }
        else if (0xfe00 <= address && address <= 0xfe9f) // oam
        {

        }
        else if (0xfea0 <= address && address <= 0xfeff) // not usable
        {

        }
        else if (0xff00 <= address && address <= 0xff7f) // i/o registers
        {

        }
        else if (0xff80 <= address && address <= 0xfffe) // hram
        {

        }
        else if (0xffff == address) // interrupt enable register
        {

        }
    }
}