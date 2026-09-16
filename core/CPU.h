#pragma once

#include <cstdint>
#include "Registers.h"

namespace qgb
{
    class CPU
    {
    public:
        CPU() = default;
        void step();

    private:
        Registers mRegisters;
    };
}

