#pragma once

#include <cstdint>

namespace qgb
{
    enum class Flag
    {
        Carry = 4,
        HalfCarry,
        Subtraction,
        Zero
    };

    class Registers
    {
    public:
        Registers() = default;

        uint16_t getAF() const;
        uint16_t getBC() const;
        uint16_t getDE() const;
        uint16_t getHL() const;
        uint16_t getSP() const;
        uint16_t getPC() const;

        uint8_t getA() const;
        uint8_t getB() const;
        uint8_t getC() const;
        uint8_t getD() const;
        uint8_t getE() const;
        uint8_t getF() const;
        uint8_t getH() const;
        uint8_t getL() const;

        bool getFlag(Flag flag) const;
        void setFlag(Flag flag, bool on);

    private:
        uint8_t mA = 0;
        uint8_t mB = 0;
        uint8_t mC = 0;
        uint8_t mD = 0;
        uint8_t mE = 0;
        uint8_t mF = 0;
        uint8_t mH = 0;
        uint8_t mL = 0;
        uint16_t mPC = 0;
        uint16_t mSP = 0;
    };
}

