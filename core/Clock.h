#pragma once

#include "IClockable.h"
#include <vector>

namespace qgb
{
class Clock
{
public:
    Clock() = default;
    void tickM();

private:
    std::vector<IClockable *> mDevices;
};
} // namespace qgb
