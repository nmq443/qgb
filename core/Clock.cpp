#include "Clock.h"

namespace qgb
{
void Clock::addDevice(IClockable& device)
{
    mDevices.push_back(&device);
}

void Clock::tickM()
{
    for (auto &device : mDevices)
    {
        device->tick(4);
    }
}

Clock::~Clock()
{
    for (auto &device : mDevices)
    {
        device = nullptr;
    }
}
} // namespace qgb