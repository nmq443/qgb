#include "Joypad.h"
#include "Utils.h"

namespace qgb
{
auto Joypad::getJp() const -> uint8_t
{
    uint8_t ret = 0b1100'0000 | (mJp & 0b0011'0000) | 0b0000'1111;
    if (!getBit(mJp, JoypadFlag::SelectDPad))
    {
        ret &= mDirections;
    }
    if (!getBit(mJp, JoypadFlag::SelectButtons))
    {
        ret &= mActions;
    }
    return ret;
}

void Joypad::setJp(uint8_t value)
{
    mJp = value & 0b0011'0000;
}

void Joypad::setDirections(uint8_t directions)
{
    mDirections = directions;
}
void Joypad::setActions(uint8_t actions)
{
    mActions = actions;
}

void Joypad::setState(JoypadState state)
{
    uint8_t directions = ~((state.down << 3) | (state.up << 2) | (state.left << 1) | state.right) & 0x0f;
    setDirections(0xf0 | directions);

    uint8_t actions = ~((state.start << 3) | (state.select << 2) | (state.b << 1) | state.a) & 0x0f;
    setActions(0xf0 | actions);
}
} // namespace qgb
