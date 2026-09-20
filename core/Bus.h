#pragma once

#include <cstdint>

namespace qgb
{
    class Bus
    {
    public:
        Bus() = default;
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);
    };
}

