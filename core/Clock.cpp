#include "Clock.h"

namespace qgb
{
void Clock::tickM()
{
    for (auto &device : mDevices)
    {
        device->tick(4);
    }
}
} // namespace qgb