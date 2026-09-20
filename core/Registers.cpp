#include "Registers.h"
#include "Utils.h"

namespace qgb
{
    uint8_t Registers::getA() const
    {
        return mA;
    }

    uint8_t Registers::getB() const
    {
        return mB;
    }

    uint8_t Registers::getC() const
    {
        return mC;
    }

    uint8_t Registers::getD() const
    {
        return mD;
    }

    uint8_t Registers::getE() const
    {
        return mE;
    }

    uint8_t Registers::getF() const
    {
        return mF;
    }

    uint8_t Registers::getH() const
    {
        return mH;
    }

    uint8_t Registers::getL() const
    {
        return mL;
    }

    uint16_t Registers::getAF() const
    {
        return makeWord(getA(), getF());
    }

    uint16_t Registers::getBC() const
    {
        return makeWord(getB(), getC());
    }

    uint16_t Registers::getDE() const
    {
        return makeWord(getD(), getE());
    }

    uint16_t Registers::getHL() const
    {
        return makeWord(getH(), getL());
    }

    uint16_t Registers::getPC() const
    {
        return mPC;
    }

    uint16_t Registers::getSP() const
    {
        return mSP;
    }

    bool Registers::getFlag(Flag flag) const
    {
        return ((mF >> static_cast<int>(flag)) & 1) > 0;
    }

    void Registers::setFlag(Flag flag, bool on)
    {
        if (on)
        {
            mF |= 1 << static_cast<int>(flag);
        }
        else
        {
            mF &= ~(1 << static_cast<int>(flag));
        }
    }

    void Registers::setPC(uint16_t pc)
    {
        mPC = pc;
    }

    void Registers::setAF(uint16_t af)
    {
        mA = af >> 8;
        mF = af & 0xf0;
    }

    void Registers::setBC(uint16_t bc)
    {
        mB = bc >> 8;
        mC = bc & 0xff;
    }

    void Registers::setDE(uint16_t de)
    {
        mD = de >> 8;
        mE = de & 0xff;
    }

    void Registers::setHL(uint16_t hl)
    {
        mH = hl >> 8;
        mL = hl & 0xff;
    }

    void Registers::setSP(uint16_t sp)
    {
        mSP = sp;
    }

    void Registers::setA(uint8_t a)
    {
        mA = a;
    }

    void Registers::setB(uint8_t b)
    {
        mB = b;
    }

    void Registers::setC(uint8_t c)
    {
        mC = c;
    }

    void Registers::setD(uint8_t d)
    {
        mD = d;
    }

    void Registers::setE(uint8_t e)
    {
        mE = e;
    }

    void Registers::setF(uint8_t f)
    {
        mF = f;
    }

    void Registers::setH(uint8_t h)
    {
        mH = h;
    }

    void Registers::setL(uint8_t l)
    {
        mL = l;
    }
}