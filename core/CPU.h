#pragma once

#include "Bus.h"
#include "Registers.h"
#include <cstdint>

namespace qgb
{
class CPU
{
public:
    CPU() = default;
    void step(Bus &bus);

private:
    uint8_t fetchByte(Bus &bus);
    uint16_t fetchWord(Bus &bus);
    uint8_t increment(uint8_t r8);
    uint8_t decrement(uint8_t r8);
    uint16_t add(uint16_t first, uint16_t second);
    uint8_t add(uint8_t first, uint8_t second);
    uint8_t addCarry(uint8_t first, uint8_t second);

private:
    Registers mRegisters;
};
} // namespace qgb
