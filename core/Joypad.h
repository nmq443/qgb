#pragma once

#include <cstdint>

namespace qgb
{
enum class JoypadFlag : uint8_t
{
    AOrRight,
    BOrLeft,
    SelectOrUp,
    StartOrDown,
    SelectDPad,
    SelectButtons
};

struct JoypadState
{
    bool a = false;
    bool b = false;
    bool start = false;
    bool select = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
};

class Joypad
{
public:
    Joypad() = default;
    auto getJp() const -> uint8_t;
    void setJp(uint8_t value);
    void setState(JoypadState state);

private:
    void setDirections(uint8_t directions);
    void setActions(uint8_t actions);

    uint8_t mJp = 0b1100'1111;
    uint8_t mDirections = 0b1111'1111; // nothing is pressed
    uint8_t mActions = 0b1111'1111;    // nothing is pressed
};
} // namespace qgb
