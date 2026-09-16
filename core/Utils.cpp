#include "Utils.h"

namespace qgb
{
    uint16_t makeWord(uint8_t low, uint8_t high)
    {
        return high << 8 | low;
    }
}