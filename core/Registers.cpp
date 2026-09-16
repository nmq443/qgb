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

}