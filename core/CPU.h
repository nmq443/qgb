#pragma once

#include <cstdint>
#include "Registers.h"
#include "Bus.h"

namespace qgb
{
    class CPU
    {
    public:
        CPU() = default;
        void step(Bus& bus);
        uint8_t fetchByte(Bus& bus);
        uint16_t fetchWord(Bus& bus);

    private:
        Registers mRegisters;
    };
}

