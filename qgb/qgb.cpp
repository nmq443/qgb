#include <iostream>

#include "core/Emulator.h"

int main()
{
    using namespace qgb;

    Emulator emu;
    //emu.load("../roms/instr_timing/instr_timing.gb");
    emu.load("../roms/cpu_instrs/individual/02-interrupts.gb");
    emu.run();
}