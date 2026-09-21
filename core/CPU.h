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
    private:
        uint8_t fetchByte(Bus& bus);
        uint16_t fetchWord(Bus& bus);
        uint8_t increment(uint8_t r8);
        uint8_t decrement(uint8_t r8);
    private:
        Registers mRegisters;
    };
}

