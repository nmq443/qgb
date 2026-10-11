#pragma once

#include "IClockable.h"
#include <vector>

namespace qgb
{
class Clock
{
public:
    Clock() = default;
    void addDevice(IClockable& device);
    void tickM();
    ~Clock();

private:
    std::vector<IClockable *> mDevices;
};
} // namespace qgb
