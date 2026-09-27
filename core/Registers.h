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

    void setA(uint8_t a);
    void setB(uint8_t b);
    void setC(uint8_t c);
    void setD(uint8_t d);
    void setE(uint8_t e);
    void setF(uint8_t f);
    void setH(uint8_t h);
    void setL(uint8_t l);

    void setPC(uint16_t pc);
    void setAF(uint16_t af);
    void setBC(uint16_t bc);
    void setDE(uint16_t de);
    void setHL(uint16_t hl);
    void setSP(uint16_t sp);

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
    uint16_t mPC = 0x0100;
    uint16_t mSP = 0;
};
} // namespace qgb
