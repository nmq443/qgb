#pragma once

namespace qgb
{
class IClockable
{
public:
    virtual void tick(int tCycles) = 0;
    virtual ~IClockable() = default;
};
} // namespace qgb