#include "CPU.h"
#include "Utils.h"
#include <cstdint>
#include <stdexcept>

namespace qgb
{
void CPU::step(Bus &bus)
{
    uint8_t opcode = fetchByte(bus);
    switch (opcode)
    {
    case 0x00: // NOP
    {
        break;
    }
    case 0x02: // LD [BC], A
    {
        bus.write(mRegisters.getBC(), mRegisters.getA());
        break;
    }
    case 0x12: // LD [DE], A
    {
        bus.write(mRegisters.getDE(), mRegisters.getA());
        break;
    }
    case 0x22: // LD [HL+], A
    {
        bus.write(mRegisters.getHL(), mRegisters.getA());
        mRegisters.setHL(static_cast<uint16_t>(mRegisters.getHL() + 1));
        break;
    }
    case 0x32: // LD [HL-], A
    {
        bus.write(mRegisters.getHL(), mRegisters.getA());
        mRegisters.setHL(static_cast<uint16_t>(mRegisters.getHL() - 1));
        break;
    }
    case 0x06: // LD B, d8
    {
        mRegisters.setB(fetchByte(bus));
        break;
    }
    case 0x16: // LD D, d8
    {
        mRegisters.setD(fetchByte(bus));
        break;
    }
    case 0x26: // LD H, d8
    {
        mRegisters.setH(fetchByte(bus));
        break;
    }
    case 0x36: // LD (HL), d8
    {
        bus.write(mRegisters.getHL(), fetchByte(bus));
        break;
    }
    case 0x0a: // LD A, (BC)
    {
        mRegisters.setA(bus.read(mRegisters.getBC()));
        break;
    }
    case 0x1a: // LD A, (DE)
    {
        mRegisters.setA(bus.read(mRegisters.getDE()));
        break;
    }
    case 0x2a: // LD A, (HL+)
    {
        mRegisters.setA(bus.read(mRegisters.getHL()));
        mRegisters.setHL(static_cast<uint16_t>(mRegisters.getHL() + 1));
        break;
    }
    case 0x3a: // LD A, (HL-)
    {
        mRegisters.setA(bus.read(mRegisters.getHL()));
        mRegisters.setHL(static_cast<uint16_t>(mRegisters.getHL() - 1));
        break;
    }
    case 0x0e: // LD C, d8
    {
        mRegisters.setC(fetchByte(bus));
        break;
    }
    case 0x1e: // LD E, d8
    {
        mRegisters.setE(fetchByte(bus));
        break;
    }
    case 0x2e: // LD L, d8
    {
        mRegisters.setL(fetchByte(bus));
        break;
    }
    case 0x3e: // LD A, d8
    {
        mRegisters.setA(fetchByte(bus));
        break;
    }
    case 0x40: // LD B, B
    {
        break;
    }
    case 0x41: // LD B, C
    {
        mRegisters.setB(mRegisters.getC());
        break;
    }
    case 0x42: // LD B, D
    {
        mRegisters.setB(mRegisters.getD());
        break;
    }
    case 0x43: // LD B, E
    {
        mRegisters.setB(mRegisters.getE());
        break;
    }
    case 0x44: // LD B, H
    {
        mRegisters.setB(mRegisters.getH());
        break;
    }
    case 0x45: // LD B, L
    {
        mRegisters.setB(mRegisters.getL());
        break;
    }
    case 0x46: // LD B, (HL)
    {
        mRegisters.setB(bus.read(mRegisters.getHL()));
        break;
    }
    case 0x47: // LD B, A
    {
        mRegisters.setB(mRegisters.getA());
        break;
    }
    case 0x48: // LD C, B
    {
        mRegisters.setC(mRegisters.getB());
        break;
    }
    case 0x49: // LD C, C
    {
        break;
    }
    case 0x4a: // LD C, D
    {
        mRegisters.setC(mRegisters.getD());
        break;
    }
    case 0x4b: // LD C, E
    {
        mRegisters.setC(mRegisters.getE());
        break;
    }
    case 0x4c: // LD C, H
    {
        mRegisters.setC(mRegisters.getH());
        break;
    }
    case 0x4d: // LD C, L
    {
        mRegisters.setC(mRegisters.getL());
        break;
    }
    case 0x4e: // LD C, (HL)
    {
        mRegisters.setC(bus.read(mRegisters.getHL()));
        break;
    }
    case 0x4f: // LD C, A
    {
        mRegisters.setC(mRegisters.getA());
        break;
    }
    case 0x50: // LD D, B
    {
        mRegisters.setD(mRegisters.getB());
        break;
    }
    case 0x51: // LD D, C
    {
        mRegisters.setD(mRegisters.getC());
        break;
    }
    case 0x52: // LD D, D
    {
        break;
    }
    case 0x53: // LD D, E
    {
        mRegisters.setD(mRegisters.getE());
        break;
    }
    case 0x54: // LD D, H
    {
        mRegisters.setD(mRegisters.getH());
        break;
    }
    case 0x55: // LD D, L
    {
        mRegisters.setD(mRegisters.getL());
        break;
    }
    case 0x56: // LD D, (HL)
    {
        mRegisters.setD(bus.read(mRegisters.getHL()));
        break;
    }
    case 0x57: // LD D, A
    {
        mRegisters.setD(mRegisters.getA());
        break;
    }
    case 0x58: // LD E, B
    {
        mRegisters.setE(mRegisters.getB());
        break;
    }
    case 0x59: // LD E, C
    {
        mRegisters.setE(mRegisters.getC());
        break;
    }
    case 0x5a: // LD E, D
    {
        mRegisters.setE(mRegisters.getD());
        break;
    }
    case 0x5b: // LD E, E
    {
        break;
    }
    case 0x5c: // LD E, H
    {
        mRegisters.setE(mRegisters.getH());
        break;
    }
    case 0x5d: // LD E, L
    {
        mRegisters.setE(mRegisters.getL());
        break;
    }
    case 0x5e: // LD E, (HL)
    {
        mRegisters.setE(bus.read(mRegisters.getHL()));
        break;
    }
    case 0x5f: // LD E, A
    {
        mRegisters.setE(mRegisters.getA());
        break;
    }
    case 0x60: // LD H, B
    {
        mRegisters.setH(mRegisters.getB());
        break;
    }
    case 0x61: // LD H, C
    {
        mRegisters.setH(mRegisters.getC());
        break;
    }
    case 0x62: // LD H, D
    {
        mRegisters.setH(mRegisters.getD());
        break;
    }
    case 0x63: // LD H, E
    {
        mRegisters.setH(mRegisters.getE());
        break;
    }
    case 0x64: // LD H, H
    {
        break;
    }
    case 0x65: // LD H, L
    {
        mRegisters.setH(mRegisters.getL());
        break;
    }
    case 0x66: // LD H, (HL)
    {
        mRegisters.setH(bus.read(mRegisters.getHL()));
        break;
    }
    case 0x67: // LD H, A
    {
        mRegisters.setH(mRegisters.getA());
        break;
    }
    case 0x68: // LD L, B
    {
        mRegisters.setL(mRegisters.getB());
        break;
    }
    case 0x69: // LD L, C
    {
        mRegisters.setL(mRegisters.getC());
        break;
    }
    case 0x6a: // LD L, D
    {
        mRegisters.setL(mRegisters.getD());
        break;
    }
    case 0x6b: // LD L, E
    {
        mRegisters.setL(mRegisters.getE());
        break;
    }
    case 0x6c: // LD L, H
    {
        mRegisters.setL(mRegisters.getH());
        break;
    }
    case 0x6d: // LD L, L
    {
        break;
    }
    case 0x6e: // LD L, (HL)
    {
        mRegisters.setL(bus.read(mRegisters.getHL()));
        break;
    }
    case 0x6f: // LD L, A
    {
        mRegisters.setL(mRegisters.getA());
        break;
    }
    case 0x70: // LD (HL), B
    {
        bus.write(mRegisters.getHL(), mRegisters.getB());
        break;
    }
    case 0x71: // LD (HL), C
    {
        bus.write(mRegisters.getHL(), mRegisters.getC());
        break;
    }
    case 0x72: // LD (HL), D
    {
        bus.write(mRegisters.getHL(), mRegisters.getD());
        break;
    }
    case 0x73: // LD (HL), E
    {
        bus.write(mRegisters.getHL(), mRegisters.getE());
        break;
    }
    case 0x74: // LD (HL), H
    {
        bus.write(mRegisters.getHL(), mRegisters.getH());
        break;
    }
    case 0x75: // LD (HL), L
    {
        bus.write(mRegisters.getHL(), mRegisters.getL());
        break;
    }
    case 0x77: // LD (HL), A
    {
        bus.write(mRegisters.getHL(), mRegisters.getA());
        break;
    }
    case 0x78: // LD A, B
    {
        mRegisters.setA(mRegisters.getB());
        break;
    }
    case 0x79: // LD A, C
    {
        mRegisters.setA(mRegisters.getC());
        break;
    }
    case 0x7a: // LD A, D
    {
        mRegisters.setA(mRegisters.getD());
        break;
    }
    case 0x7b: // LD A, E
    {
        mRegisters.setA(mRegisters.getE());
        break;
    }
    case 0x7c: // LD A, H
    {
        mRegisters.setA(mRegisters.getH());
        break;
    }
    case 0x7d: // LD A, L
    {
        mRegisters.setA(mRegisters.getL());
        break;
    }
    case 0x7e: // LD A, (HL)
    {
        mRegisters.setA(bus.read(mRegisters.getHL()));
        break;
    }
    case 0x7f: // LD A, A
    {
        break;
    }
    case 0xe0: // LD (a8), A
    {
        uint16_t address = fetchByte(bus) + 0xff00;
        bus.write(address, mRegisters.getA());
        break;
    }
    case 0xf0: // LD A, (a8)
    {
        uint16_t address = fetchByte(bus) + 0xff00;
        mRegisters.setA(bus.read(address));
        break;
    }
    case 0xe2: // LD (C), A
    {
        uint16_t address = mRegisters.getC() + 0xff00;
        bus.write(address, mRegisters.getA());
        break;
    }
    case 0xf2: // LD A, (C)
    {
        uint16_t address = mRegisters.getC() + 0xff00;
        mRegisters.setA(bus.read(address));
        break;
    }
    case 0xea: // LD (a16), A
    {
        uint16_t address = fetchWord(bus);
        bus.write(address, mRegisters.getA());
        break;
    }
    case 0xfa: // LD A, (a16)
    {
        uint16_t address = fetchWord(bus);
        mRegisters.setA(bus.read(address));
        break;
    }
    case 0x01: // LD BC, d16
    {
        uint16_t d16 = fetchWord(bus);
        mRegisters.setBC(d16);
        break;
    }
    case 0x11: // LD DE, d16
    {
        uint16_t d16 = fetchWord(bus);
        mRegisters.setDE(d16);
        break;
    }
    case 0x21: // LD HL, d16
    {
        uint16_t d16 = fetchWord(bus);
        mRegisters.setHL(d16);
        break;
    }
    case 0x31: // LD SP, d16
    {
        uint16_t d16 = fetchWord(bus);
        mRegisters.setSP(d16);
        break;
    }
    case 0x08: // LD (a16), SP
    {
        uint16_t a16 = fetchWord(bus);
        auto [high, low] = makeBytes(mRegisters.getSP());
        bus.write(a16, low);
        bus.write(a16 + 1, high);
        break;
    }
    case 0xf8: // LD HL, SP+s8
    {
        int8_t s8 = static_cast<int8_t>(fetchByte(bus));
        int32_t sum = static_cast<int32_t>(mRegisters.getSP()) + s8;
        bool halfCarryFlag = (mRegisters.getSP() & 0x0f) + (s8 & 0x0f) > 0x0f;
        bool carryFlag = (mRegisters.getSP() & 0xff) + static_cast<uint8_t>(s8) > 0xff;
        mRegisters.setHL(static_cast<uint16_t>(sum));

        mRegisters.setFlag(Flag::Zero, false);
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, halfCarryFlag);
        mRegisters.setFlag(Flag::Carry, carryFlag);
        break;
    }
    case 0xf9: // LD SP, HL
    {
        mRegisters.setSP(mRegisters.getHL());
        break;
    }
    case 0x03: // INC BC
    {
        mRegisters.setBC(static_cast<uint16_t>(mRegisters.getBC() + 1));
        break;
    }
    case 0x13: // INC DE
    {
        mRegisters.setDE(static_cast<uint16_t>(mRegisters.getDE() + 1));
        break;
    }
    case 0x23: // INC HL
    {
        mRegisters.setHL(static_cast<uint16_t>(mRegisters.getHL() + 1));
        break;
    }
    case 0x33: // INC SP
    {
        mRegisters.setSP(static_cast<uint16_t>(mRegisters.getSP() + 1));
        break;
    }
    case 0x04: // INC B
    {
        mRegisters.setB(increment(mRegisters.getB()));
        break;
    }
    case 0x14: // INC D
    {
        mRegisters.setD(increment(mRegisters.getD()));
        break;
    }
    case 0x24: // INC H
    {
        mRegisters.setH(increment(mRegisters.getH()));
        break;
    }
    case 0x34: // INC (HL)
    {
        uint8_t val = bus.read(mRegisters.getHL());
        bus.write(mRegisters.getHL(), increment(val));
        break;
    }
    case 0x0c: // INC C
    {
        mRegisters.setC(increment(mRegisters.getC()));
        break;
    }
    case 0x1c: // INC E
    {
        mRegisters.setE(increment(mRegisters.getE()));
        break;
    }
    case 0x2c: // INC L
    {
        mRegisters.setL(increment(mRegisters.getL()));
        break;
    }
    case 0x3c: // INC A
    {
        mRegisters.setA(increment(mRegisters.getA()));
        break;
    }
    case 0x05: // DEC B
    {
        mRegisters.setB(decrement(mRegisters.getB()));
        break;
    }
    case 0x15: // DEC D
    {
        mRegisters.setD(decrement(mRegisters.getD()));
        break;
    }
    case 0x25: // DEC H
    {
        mRegisters.setH(decrement(mRegisters.getH()));
        break;
    }
    case 0x0d: // DEC C
    {
        mRegisters.setC(decrement(mRegisters.getC()));
        break;
    }
    case 0x1d: // DEC E
    {
        mRegisters.setE(decrement(mRegisters.getE()));
        break;
    }
    case 0x2d: // DEC L
    {
        mRegisters.setL(decrement(mRegisters.getL()));
        break;
    }
    case 0x3d: // DEC A
    {
        mRegisters.setA(decrement(mRegisters.getA()));
        break;
    }
    case 0x35: // DEC (HL)
    {
        uint8_t val = bus.read(mRegisters.getHL());
        bus.write(mRegisters.getHL(), decrement(val));
        break;
    }
    case 0x0b: // DEC BC
    {
        mRegisters.setBC(static_cast<uint16_t>(mRegisters.getBC() - 1));
        break;
    }
    case 0x1b: // DEC DE
    {
        mRegisters.setDE(static_cast<uint16_t>(mRegisters.getDE() - 1));
        break;
    }
    case 0x2b: // DEC HL
    {
        mRegisters.setHL(static_cast<uint16_t>(mRegisters.getHL() - 1));
        break;
    }
    case 0x3b: // DEC SP
    {
        mRegisters.setSP(static_cast<uint16_t>(mRegisters.getSP() - 1));
        break;
    }
    case 0x20: // JR NZ, s8
    {
        int8_t s8 = fetchByte(bus);
        if (!mRegisters.getFlag(Flag::Zero))
        {
            mRegisters.setPC(mRegisters.getPC() + s8);
        }
        break;
    }
    case 0x30: // JR NC, s8
    {
        int8_t s8 = fetchByte(bus);
        if (!mRegisters.getFlag(Flag::Carry))
        {
            mRegisters.setPC(mRegisters.getPC() + s8);
        }
        break;
    }
    case 0x18: // JR s8
    {
        int8_t s8 = fetchByte(bus);
        mRegisters.setPC(mRegisters.getPC() + s8);
        break;
    }
    case 0x28: // JR Z, s8
    {
        int8_t s8 = fetchByte(bus);
        if (mRegisters.getFlag(Flag::Zero))
        {
            mRegisters.setPC(mRegisters.getPC() + s8);
        }
        break;
    }
    case 0x38: // JR C, s8
    {
        int8_t s8 = fetchByte(bus);
        if (mRegisters.getFlag(Flag::Carry))
        {
            mRegisters.setPC(mRegisters.getPC() + s8);
        }
        break;
    }
    case 0x07: // RLCA
    {
        uint8_t a = mRegisters.getA();
        uint8_t bit7 = a >> 7;
        a = (a << 1) | bit7;
        mRegisters.setA(a);

        mRegisters.setFlag(Flag::Zero, false);
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, false);
        mRegisters.setFlag(Flag::Carry, bit7 == 0x01);
        break;
    }
    case 0x17: // RLA
    {
        uint8_t a = mRegisters.getA();
        uint8_t bit7 = (a >> 7) & 0x01;
        uint8_t oldCarry = mRegisters.getFlag(Flag::Carry) ? 1 : 0;

        a = (a << 1) | oldCarry;
        mRegisters.setA(a);

        mRegisters.setFlag(Flag::Zero, false);
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, false);
        mRegisters.setFlag(Flag::Carry, bit7 == 1);
        break;
    }
    case 0x0f: // RRCA
    {
        uint8_t a = mRegisters.getA();
        uint8_t bit0 = a & 0x01;
        a = (a >> 1) | (bit0 << 7);
        mRegisters.setA(a);

        mRegisters.setFlag(Flag::Zero, false);
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, false);
        mRegisters.setFlag(Flag::Carry, bit0 == 0x01);
        break;
    }
    case 0x1f: // RRA
    {
        uint8_t a = mRegisters.getA();
        uint8_t bit0 = a & 0x01;
        uint8_t oldCarry = mRegisters.getFlag(Flag::Carry) ? 1 : 0;

        a = (a >> 1) | (oldCarry << 7);
        mRegisters.setA(a);

        mRegisters.setFlag(Flag::Zero, false);
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, false);
        mRegisters.setFlag(Flag::Carry, bit0 == 1);
        break;
    }
    case 0x09: // ADD HL, BC
    {
        mRegisters.setHL(add(mRegisters.getHL(), mRegisters.getBC()));
        break;
    }
    case 0x19: // ADD HL, DE
    {
        mRegisters.setHL(add(mRegisters.getHL(), mRegisters.getDE()));
        break;
    }
    case 0x29: // ADD HL, HL
    {
        mRegisters.setHL(add(mRegisters.getHL(), mRegisters.getHL()));
        break;
    }
    case 0x39: // ADD HL, SP
    {
        mRegisters.setHL(add(mRegisters.getHL(), mRegisters.getSP()));
        break;
    }
    case 0x80: // ADD A, B
    {
        mRegisters.setA(add(mRegisters.getA(), mRegisters.getB()));
        break;
    }
    case 0x81: // ADD A, C
    {
        mRegisters.setA(add(mRegisters.getA(), mRegisters.getC()));
        break;
    }
    case 0x82: // ADD A, D
    {
        mRegisters.setA(add(mRegisters.getA(), mRegisters.getD()));
        break;
    }
    case 0x83: // ADD A, E
    {
        mRegisters.setA(add(mRegisters.getA(), mRegisters.getE()));
        break;
    }
    case 0x84: // ADD A, H
    {
        mRegisters.setA(add(mRegisters.getA(), mRegisters.getH()));
        break;
    }
    case 0x85: // ADD A, L
    {
        mRegisters.setA(add(mRegisters.getA(), mRegisters.getL()));
        break;
    }
    case 0x86: // ADD A, (HL)
    {
        mRegisters.setA(add(mRegisters.getA(), bus.read(mRegisters.getHL())));
        break;
    }
    case 0x87: // ADD A, A
    {
        mRegisters.setA(add(mRegisters.getA(), mRegisters.getA()));
        break;
    }
    case 0xc6: // ADD A, d8
    {
        mRegisters.setA(add(mRegisters.getA(), fetchByte(bus)));
        break;
    }
    case 0xe8: // ADD SP, s8
    {
        int8_t s8 = fetchByte(bus);
        uint8_t u8 = static_cast<uint8_t>(s8);

        uint16_t sp = mRegisters.getSP();
        bool setHalfCarry = (sp & 0x0f) + (u8 & 0x0f) > 0x0f;
        bool setCarry = (sp & 0xff) + u8 > 0xff;
        mRegisters.setSP(static_cast<uint16_t>(sp + s8));

        mRegisters.setFlag(Flag::Zero, false);
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, setHalfCarry);
        mRegisters.setFlag(Flag::Carry, setCarry);
        break;
    }
    case 0x27: // DAA
    {
        uint8_t adjustment = 0;
        uint8_t a = mRegisters.getA();
        bool setCarry = false;

        if (mRegisters.getFlag(Flag::Subtraction))
        {
            if (mRegisters.getFlag(Flag::HalfCarry))
            {
                adjustment += 0x06;
            }
            if (mRegisters.getFlag(Flag::Carry))
            {
                adjustment += 0x60;
                setCarry = true;
            }
            a -= adjustment;
        }
        else
        {
            if (mRegisters.getFlag(Flag::HalfCarry) or ((a & 0x0f) > 0x09))
            {
                adjustment += 0x06;
            }
            if (mRegisters.getFlag(Flag::Carry) or (a > 0x99))
            {
                adjustment += 0x60;
                setCarry = true;
            }
            a += adjustment;
        }

        mRegisters.setFlag(Flag::Zero, a == 0);
        mRegisters.setFlag(Flag::HalfCarry, false);
        mRegisters.setFlag(Flag::Carry, setCarry);
        mRegisters.setA(a);
        break;
    }
    case 0x37: // SCF
    {
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, false);
        mRegisters.setFlag(Flag::Carry, true);
        break;
    }
    case 0x2f: // CPL
    {
        mRegisters.setA(~mRegisters.getA());
        mRegisters.setFlag(Flag::Subtraction, true);
        mRegisters.setFlag(Flag::HalfCarry, true);
        break;
    }
    case 0x3f: // CCF
    {
        mRegisters.setFlag(Flag::Subtraction, false);
        mRegisters.setFlag(Flag::HalfCarry, false);
        mRegisters.setFlag(Flag::Carry, !mRegisters.getFlag(Flag::Carry));
        break;
    }
    case 0x88: // ADC A, B
    {
        mRegisters.setA(addCarry(mRegisters.getA(), mRegisters.getB()));
        break;
    }
    case 0x89: // ADC A, C
    {
        mRegisters.setA(addCarry(mRegisters.getA(), mRegisters.getC()));
        break;
    }
    case 0x8a: // ADC A, D
    {
        mRegisters.setA(addCarry(mRegisters.getA(), mRegisters.getD()));
        break;
    }
    case 0x8b: // ADC A, E
    {
        mRegisters.setA(addCarry(mRegisters.getA(), mRegisters.getE()));
        break;
    }
    case 0x8c: // ADC A, H
    {
        mRegisters.setA(addCarry(mRegisters.getA(), mRegisters.getH()));
        break;
    }
    case 0x8d: // ADC A, L
    {
        mRegisters.setA(addCarry(mRegisters.getA(), mRegisters.getL()));
        break;
    }
    case 0x8e: // ADC A, (HL)
    {
        mRegisters.setA(addCarry(mRegisters.getA(), bus.read(mRegisters.getHL())));
        break;
    }
    case 0x8f: // ADC A, A
    {
        mRegisters.setA(addCarry(mRegisters.getA(), mRegisters.getA()));
        break;
    }
    case 0xce: // ADC A, d8
    {
        mRegisters.setA(addCarry(mRegisters.getA(), fetchByte(bus)));
        break;
    }
    case 0x90: // SUB B
    {
        mRegisters.setA(sub(mRegisters.getA(), mRegisters.getB()));
        break;
    }
    case 0x91: // SUB C
    {
        mRegisters.setA(sub(mRegisters.getA(), mRegisters.getC()));
        break;
    }
    case 0x92: // SUB D
    {
        mRegisters.setA(sub(mRegisters.getA(), mRegisters.getD()));
        break;
    }
    case 0x93: // SUB E
    {
        mRegisters.setA(sub(mRegisters.getA(), mRegisters.getE()));
        break;
    }
    case 0x94: // SUB H
    {
        mRegisters.setA(sub(mRegisters.getA(), mRegisters.getH()));
        break;
    }
    case 0x95: // SUB L
    {
        mRegisters.setA(sub(mRegisters.getA(), mRegisters.getL()));
        break;
    }
    case 0x96: // SUB (HL)
    {
        mRegisters.setA(sub(mRegisters.getA(), bus.read(mRegisters.getHL())));
        break;
    }
    case 0x97: // SUB A
    {
        mRegisters.setA(sub(mRegisters.getA(), mRegisters.getA()));
        break;
    }
    case 0xd6: // SUB d8
    {
        mRegisters.setA(sub(mRegisters.getA(), fetchByte(bus)));
        break;
    }
    case 0x98: // SBC A, B
    {
        mRegisters.setA(subCarry(mRegisters.getA(), mRegisters.getB()));
        break;
    }
    case 0x99: // SBC A, C
    {
        mRegisters.setA(subCarry(mRegisters.getA(), mRegisters.getC()));
        break;
    }
    case 0x9a: // SBC A, D
    {
        mRegisters.setA(subCarry(mRegisters.getA(), mRegisters.getD()));
        break;
    }
    case 0x9b: // SBC A, E
    {
        mRegisters.setA(subCarry(mRegisters.getA(), mRegisters.getE()));
        break;
    }
    case 0x9c: // SBC A, H
    {
        mRegisters.setA(subCarry(mRegisters.getA(), mRegisters.getH()));
        break;
    }
    case 0x9d: // SBC A, L
    {
        mRegisters.setA(subCarry(mRegisters.getA(), mRegisters.getL()));
        break;
    }
    case 0x9e: // SBC A, (HL)
    {
        mRegisters.setA(subCarry(mRegisters.getA(), bus.read(mRegisters.getHL())));
        break;
    }
    case 0x9f: // SBC A, A
    {
        mRegisters.setA(subCarry(mRegisters.getA(), mRegisters.getA()));
        break;
    }
    case 0xde: // SBC A, d8
    {
        mRegisters.setA(subCarry(mRegisters.getA(), fetchByte(bus)));
        break;
    }
    case 0xa0: // AND A, B
    {
        mRegisters.setA(opAnd(mRegisters.getA(), mRegisters.getB()));
        break;
    }
    case 0xa1: // AND A, C
    {
        mRegisters.setA(opAnd(mRegisters.getA(), mRegisters.getC()));
        break;
    }
    case 0xa2: // AND A, D
    {
        mRegisters.setA(opAnd(mRegisters.getA(), mRegisters.getD()));
        break;
    }
    case 0xa3: // AND A, E
    {
        mRegisters.setA(opAnd(mRegisters.getA(), mRegisters.getE()));
        break;
    }
    case 0xa4: // AND A, H
    {
        mRegisters.setA(opAnd(mRegisters.getA(), mRegisters.getH()));
        break;
    }
    case 0xa5: // AND A, L
    {
        mRegisters.setA(opAnd(mRegisters.getA(), mRegisters.getL()));
        break;
    }
    case 0xa6: // AND A, (HL)
    {
        mRegisters.setA(opAnd(mRegisters.getA(), bus.read(mRegisters.getHL())));
        break;
    }
    case 0xa7: // AND A, A
    {
        mRegisters.setA(opAnd(mRegisters.getA(), mRegisters.getA()));
        break;
    }
    case 0xe6: // AND A, d8
    {
        mRegisters.setA(opAnd(mRegisters.getA(), fetchByte(bus)));
        break;
    }
    case 0xa8: // XOR A, B
    {
        mRegisters.setA(opXor(mRegisters.getA(), mRegisters.getB()));
        break;
    }
    case 0xa9: // XOR A, C
    {
        mRegisters.setA(opXor(mRegisters.getA(), mRegisters.getC()));
        break;
    }
    case 0xaa: // XOR A, D
    {
        mRegisters.setA(opXor(mRegisters.getA(), mRegisters.getD()));
        break;
    }
    case 0xab: // XOR A, E
    {
        mRegisters.setA(opXor(mRegisters.getA(), mRegisters.getE()));
        break;
    }
    case 0xac: // XOR A, H
    {
        mRegisters.setA(opXor(mRegisters.getA(), mRegisters.getH()));
        break;
    }
    case 0xad: // XOR A, L
    {
        mRegisters.setA(opXor(mRegisters.getA(), mRegisters.getL()));
        break;
    }
    case 0xae: // XOR A, (HL)
    {
        mRegisters.setA(opXor(mRegisters.getA(), bus.read(mRegisters.getHL())));
        break;
    }
    case 0xaf: // XOR A, A
    {
        mRegisters.setA(opXor(mRegisters.getA(), mRegisters.getA()));
        break;
    }
    case 0xee: // XOR A, d8
    {
        mRegisters.setA(opXor(mRegisters.getA(), fetchByte(bus)));
        break;
    }
    case 0xb0: // OR A, B
    {
        mRegisters.setA(opOr(mRegisters.getA(), mRegisters.getB()));
        break;
    }
    case 0xb1: // OR A, C
    {
        mRegisters.setA(opOr(mRegisters.getA(), mRegisters.getC()));
        break;
    }
    case 0xb2: // OR A, D
    {
        mRegisters.setA(opOr(mRegisters.getA(), mRegisters.getD()));
        break;
    }
    case 0xb3: // OR A, E
    {
        mRegisters.setA(opOr(mRegisters.getA(), mRegisters.getE()));
        break;
    }
    case 0xb4: // OR A, H
    {
        mRegisters.setA(opOr(mRegisters.getA(), mRegisters.getH()));
        break;
    }
    case 0xb5: // OR A, L
    {
        mRegisters.setA(opOr(mRegisters.getA(), mRegisters.getL()));
        break;
    }
    case 0xb6: // OR A, (HL)
    {
        mRegisters.setA(opOr(mRegisters.getA(), bus.read(mRegisters.getHL())));
        break;
    }
    case 0xb7: // OR A, A
    {
        mRegisters.setA(opOr(mRegisters.getA(), mRegisters.getA()));
        break;
    }
    case 0xf6: // OR A, d8
    {
        mRegisters.setA(opOr(mRegisters.getA(), fetchByte(bus)));
        break;
    }
    case 0xb8: // CP A, B
    {
        cp(mRegisters.getA(), mRegisters.getB());
        break;
    }
    case 0xb9: // CP A, C
    {
        cp(mRegisters.getA(), mRegisters.getC());
        break;
    }
    case 0xba: // CP A, D
    {
        cp(mRegisters.getA(), mRegisters.getD());
        break;
    }
    case 0xbb: // CP A, E
    {
        cp(mRegisters.getA(), mRegisters.getE());
        break;
    }
    case 0xbc: // CP A, H
    {
        cp(mRegisters.getA(), mRegisters.getH());
        break;
    }
    case 0xbd: // CP A, L
    {
        cp(mRegisters.getA(), mRegisters.getL());
        break;
    }
    case 0xbe: // CP A, (HL)
    {
        cp(mRegisters.getA(), bus.read(mRegisters.getHL()));
        break;
    }
    case 0xbf: // CP A, A
    {
        cp(mRegisters.getA(), mRegisters.getA());
        break;
    }
    case 0xfe: // CP A, d8
    {
        cp(mRegisters.getA(), fetchByte(bus));
        break;
    }
    case 0xc0: // RET NZ
    {
        if (!mRegisters.getFlag(Flag::Zero))
        {
            mRegisters.setPC(pop16(bus));
        }
        break;
    }
    case 0xd0: // RET NC
    {
        if (!mRegisters.getFlag(Flag::Carry))
        {
            mRegisters.setPC(pop16(bus));
        }
        break;
    }
    case 0xc8: // RET Z
    {
        if (mRegisters.getFlag(Flag::Zero))
        {
            mRegisters.setPC(pop16(bus));
        }
        break;
    }
    case 0xd8: // RET C
    {
        if (mRegisters.getFlag(Flag::Carry))
        {
            mRegisters.setPC(pop16(bus));
        }
        break;
    }
    case 0xc9: // RET
    {
        mRegisters.setPC(pop16(bus));
        break;
    }
    case 0xc1: // POP BC
    {
        mRegisters.setBC(pop16(bus));
        break;
    }
    case 0xd1: // POP DE
    {
        mRegisters.setDE(pop16(bus));
        break;
    }
    case 0xe1: // POP HL
    {
        mRegisters.setHL(pop16(bus));
        break;
    }
    case 0xf1: // POP AF
    {
        mRegisters.setAF(pop16(bus));
        break;
    }
    case 0xc2: // JP NZ, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (!mRegisters.getFlag(Flag::Zero))
        {
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xd2: // JP NC, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (!mRegisters.getFlag(Flag::Carry))
        {
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xca: // JP Z, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (mRegisters.getFlag(Flag::Zero))
        {
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xda: // JP C, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (mRegisters.getFlag(Flag::Carry))
        {
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xc3: // JP a16
    {
        uint16_t a16 = fetchWord(bus);
        mRegisters.setPC(a16);
        break;
    }
    case 0xc5: // PUSH BC
    {
        push16(bus, mRegisters.getBC());
        break;
    }
    case 0xd5: // PUSH DE
    {
        push16(bus, mRegisters.getDE());
        break;
    }
    case 0xe5: // PUSH HL
    {
        push16(bus, mRegisters.getHL());
        break;
    }
    case 0xf5: // PUSH AF
    {
        push16(bus, mRegisters.getAF());
        break;
    }
    case 0xc4: // CALL NZ, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (!mRegisters.getFlag(Flag::Zero))
        {
            push16(bus, mRegisters.getPC());
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xd4: // CALL NC, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (!mRegisters.getFlag(Flag::Carry))
        {
            push16(bus, mRegisters.getPC());
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xcc: // CALL Z, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (mRegisters.getFlag(Flag::Zero))
        {
            push16(bus, mRegisters.getPC());
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xdc: // CALL C, a16
    {
        uint16_t a16 = fetchWord(bus);
        if (mRegisters.getFlag(Flag::Carry))
        {
            push16(bus, mRegisters.getPC());
            mRegisters.setPC(a16);
        }
        break;
    }
    case 0xcd: // CALL a16
    {
        uint16_t a16 = fetchWord(bus);
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(a16);
        break;
    }
    case 0xc7: // RST 0
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0000);
        break;
    }
    case 0xd7: // RST 2
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0010);
        break;
    }
    case 0xe7: // RST 4
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0020);
        break;
    }
    case 0xf7: // RST 6
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0030);
        break;
    }
    case 0xcf: // RST 1
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0008);
        break;
    }
    case 0xdf: // RST 3
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0018);
        break;
    }
    case 0xef: // RST 5
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0028);
        break;
    }
    case 0xff: // RST 7
    {
        push16(bus, mRegisters.getPC());
        mRegisters.setPC(0x0038);
        break;
    }
    case 0x10: // STOP (0x1000)
    {
        fetchByte(bus);    // Fetch required 2nd byte (0x00)
        mIsStopped = true; // Pause CPU execution until button press/reset
        break;
    }
    case 0x76: // HALT
    {
        mHalted = true;
        break;
    }
    case 0xf3: // DI
    {
        mIME = false;
        mIMEEnablePending = false;
        mIMEEnableScheduled = false;
        break;
    }
    case 0xfb: // EI
    {
        mIMEEnablePending = true;
        break;
    }
    case 0xcb: // 16-bit opcodes
    {
        uint8_t secondByte = fetchByte(bus);
        switch (secondByte)
        {
        case 0x00: // RLC B
        {
            mRegisters.setB(rlc(mRegisters.getB()));
            break;
        }
        case 0x01: // RLC C
        {
            mRegisters.setC(rlc(mRegisters.getC()));
            break;
        }
        case 0x02: // RLC D
        {
            mRegisters.setD(rlc(mRegisters.getD()));
            break;
        }
        case 0x03: // RLC E
        {
            mRegisters.setE(rlc(mRegisters.getE()));
            break;
        }
        case 0x04: // RLC H
        {
            mRegisters.setH(rlc(mRegisters.getH()));
            break;
        }
        case 0x05: // RLC L
        {
            mRegisters.setL(rlc(mRegisters.getL()));
            break;
        }
        case 0x06: // RLC (HL)
        {
            bus.write(mRegisters.getHL(), rlc(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x07: // RLC A
        {
            mRegisters.setA(rlc(mRegisters.getA()));
            break;
        }
        case 0x10: // RL B
        {
            mRegisters.setB(rl(mRegisters.getB()));
            break;
        }
        case 0x11: // RL C
        {
            mRegisters.setC(rl(mRegisters.getC()));
            break;
        }
        case 0x12: // RL D
        {
            mRegisters.setD(rl(mRegisters.getD()));
            break;
        }
        case 0x13: // RL E
        {
            mRegisters.setE(rl(mRegisters.getE()));
            break;
        }
        case 0x14: // RL H
        {
            mRegisters.setH(rl(mRegisters.getH()));
            break;
        }
        case 0x15: // RL L
        {
            mRegisters.setL(rl(mRegisters.getL()));
            break;
        }
        case 0x16: // RL (HL)
        {
            bus.write(mRegisters.getHL(), rl(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x17: // RL A
        {
            mRegisters.setA(rl(mRegisters.getA()));
            break;
        }
        case 0x08: // RRC B
        {
            mRegisters.setB(rrc(mRegisters.getB()));
            break;
        }
        case 0x09: // RRC C
        {
            mRegisters.setC(rrc(mRegisters.getC()));
            break;
        }
        case 0x0a: // RRC D
        {
            mRegisters.setD(rrc(mRegisters.getD()));
            break;
        }
        case 0x0b: // RRC E
        {
            mRegisters.setE(rrc(mRegisters.getE()));
            break;
        }
        case 0x0c: // RRC H
        {
            mRegisters.setH(rrc(mRegisters.getH()));
            break;
        }
        case 0x0d: // RRC L
        {
            mRegisters.setL(rrc(mRegisters.getL()));
            break;
        }
        case 0x0e: // RRC (HL)
        {
            bus.write(mRegisters.getHL(), rrc(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x0f: // RRC A
        {
            mRegisters.setA(rrc(mRegisters.getA()));
            break;
        }
        case 0x18: // RR B
        {
            mRegisters.setB(rr(mRegisters.getB()));
            break;
        }
        case 0x19: // RR C
        {
            mRegisters.setC(rr(mRegisters.getC()));
            break;
        }
        case 0x1a: // RR D
        {
            mRegisters.setD(rr(mRegisters.getD()));
            break;
        }
        case 0x1b: // RR E
        {
            mRegisters.setE(rr(mRegisters.getE()));
            break;
        }
        case 0x1c: // RR H
        {
            mRegisters.setH(rr(mRegisters.getH()));
            break;
        }
        case 0x1d: // RR L
        {
            mRegisters.setL(rr(mRegisters.getL()));
            break;
        }
        case 0x1e: // RR (HL)
        {
            bus.write(mRegisters.getHL(), rr(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x1f: // RR A
        {
            mRegisters.setA(rr(mRegisters.getA()));
            break;
        }
        case 0x20: // SLA B
        {
            mRegisters.setB(sla(mRegisters.getB()));
            break;
        }
        case 0x21: // SLA C
        {
            mRegisters.setC(sla(mRegisters.getC()));
            break;
        }
        case 0x22: // SLA D
        {
            mRegisters.setD(sla(mRegisters.getD()));
            break;
        }
        case 0x23: // SLA E
        {
            mRegisters.setE(sla(mRegisters.getE()));
            break;
        }
        case 0x24: // SLA H
        {
            mRegisters.setH(sla(mRegisters.getH()));
            break;
        }
        case 0x25: // SLA L
        {
            mRegisters.setL(sla(mRegisters.getL()));
            break;
        }
        case 0x26: // SLA (HL)
        {
            bus.write(mRegisters.getHL(), sla(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x27: // SLA A
        {
            mRegisters.setA(sla(mRegisters.getA()));
            break;
        }
        case 0x28: // SRA B
        {
            mRegisters.setB(sra(mRegisters.getB()));
            break;
        }
        case 0x29: // SRA C
        {
            mRegisters.setC(sra(mRegisters.getC()));
            break;
        }
        case 0x2a: // SRA D
        {
            mRegisters.setD(sra(mRegisters.getD()));
            break;
        }
        case 0x2b: // SRA E
        {
            mRegisters.setE(sra(mRegisters.getE()));
            break;
        }
        case 0x2c: // SRA H
        {
            mRegisters.setH(sra(mRegisters.getH()));
            break;
        }
        case 0x2d: // SRA L
        {
            mRegisters.setL(sra(mRegisters.getL()));
            break;
        }
        case 0x2e: // SRA (HL)
        {
            bus.write(mRegisters.getHL(), sra(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x2f: // SRA A
        {
            mRegisters.setA(sra(mRegisters.getA()));
            break;
        }
        case 0x30: // SWAP B
        {
            mRegisters.setB(swap(mRegisters.getB()));
            break;
        }
        case 0x31: // SWAP C
        {
            mRegisters.setC(swap(mRegisters.getC()));
            break;
        }
        case 0x32: // SWAP D
        {
            mRegisters.setD(swap(mRegisters.getD()));
            break;
        }
        case 0x33: // SWAP E
        {
            mRegisters.setE(swap(mRegisters.getE()));
            break;
        }
        case 0x34: // SWAP H
        {
            mRegisters.setH(swap(mRegisters.getH()));
            break;
        }
        case 0x35: // SWAP L
        {
            mRegisters.setL(swap(mRegisters.getL()));
            break;
        }
        case 0x36: // SWAP (HL)
        {
            bus.write(mRegisters.getHL(), swap(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x37: // SWAP A
        {
            mRegisters.setA(swap(mRegisters.getA()));
            break;
        }
        case 0x38: // SRL B
        {
            mRegisters.setB(srl(mRegisters.getB()));
            break;
        }
        case 0x39: // SRL C
        {
            mRegisters.setC(srl(mRegisters.getC()));
            break;
        }
        case 0x3a: // SRL D
        {
            mRegisters.setD(srl(mRegisters.getD()));
            break;
        }
        case 0x3b: // SRL E
        {
            mRegisters.setE(srl(mRegisters.getE()));
            break;
        }
        case 0x3c: // SRL H
        {
            mRegisters.setH(srl(mRegisters.getH()));
            break;
        }
        case 0x3d: // SRL L
        {
            mRegisters.setL(srl(mRegisters.getL()));
            break;
        }
        case 0x3e: // SRL (HL)
        {
            bus.write(mRegisters.getHL(), srl(bus.read(mRegisters.getHL())));
            break;
        }
        case 0x3f: // SRL A
        {
            mRegisters.setA(srl(mRegisters.getA()));
            break;
        }
        case 0x40: // BIT 0, B
        {
            bit(mRegisters.getB(), 0);
            break;
        }
        case 0x41: // BIT 0, C
        {
            bit(mRegisters.getC(), 0);
            break;
        }
        case 0x42: // BIT 0, D
        {
            bit(mRegisters.getD(), 0);
            break;
        }
        case 0x43: // BIT 0, E
        {
            bit(mRegisters.getE(), 0);
            break;
        }
        case 0x44: // BIT 0, H
        {
            bit(mRegisters.getH(), 0);
            break;
        }
        case 0x45: // BIT 0, L
        {
            bit(mRegisters.getL(), 0);
            break;
        }
        case 0x46: // BIT 0, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 0);
            break;
        }
        case 0x47: // BIT 0, A
        {
            bit(mRegisters.getA(), 0);
            break;
        }
        case 0x48: // BIT 1, B
        {
            bit(mRegisters.getB(), 1);
            break;
        }
        case 0x49: // BIT 1, C
        {
            bit(mRegisters.getC(), 1);
            break;
        }
        case 0x4a: // BIT 1, D
        {
            bit(mRegisters.getD(), 1);
            break;
        }
        case 0x4b: // BIT 1, E
        {
            bit(mRegisters.getE(), 1);
            break;
        }
        case 0x4c: // BIT 1, H
        {
            bit(mRegisters.getH(), 1);
            break;
        }
        case 0x4d: // BIT 1, L
        {
            bit(mRegisters.getL(), 1);
            break;
        }
        case 0x4e: // BIT 1, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 1);
            break;
        }
        case 0x4f: // BIT 1, A
        {
            bit(mRegisters.getA(), 1);
            break;
        }
        case 0x50: // BIT 2, B
        {
            bit(mRegisters.getB(), 2);
            break;
        }
        case 0x51: // BIT 2, C
        {
            bit(mRegisters.getC(), 2);
            break;
        }
        case 0x52: // BIT 2, D
        {
            bit(mRegisters.getD(), 2);
            break;
        }
        case 0x53: // BIT 2, E
        {
            bit(mRegisters.getE(), 2);
            break;
        }
        case 0x54: // BIT 2, H
        {
            bit(mRegisters.getH(), 2);
            break;
        }
        case 0x55: // BIT 2, L
        {
            bit(mRegisters.getL(), 2);
            break;
        }
        case 0x56: // BIT 2, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 2);
            break;
        }
        case 0x57: // BIT 2, A
        {
            bit(mRegisters.getA(), 2);
            break;
        }
        case 0x58: // BIT 3, B
        {
            bit(mRegisters.getB(), 3);
            break;
        }
        case 0x59: // BIT 3, C
        {
            bit(mRegisters.getC(), 3);
            break;
        }
        case 0x5a: // BIT 3, D
        {
            bit(mRegisters.getD(), 3);
            break;
        }
        case 0x5b: // BIT 3, E
        {
            bit(mRegisters.getE(), 3);
            break;
        }
        case 0x5c: // BIT 3, H
        {
            bit(mRegisters.getH(), 3);
            break;
        }
        case 0x5d: // BIT 3, L
        {
            bit(mRegisters.getL(), 3);
            break;
        }
        case 0x5e: // BIT 3, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 3);
            break;
        }
        case 0x5f: // BIT 3, A
        {
            bit(mRegisters.getA(), 3);
            break;
        }
        case 0x60: // BIT 4, B
        {
            bit(mRegisters.getB(), 4);
            break;
        }
        case 0x61: // BIT 4, C
        {
            bit(mRegisters.getC(), 4);
            break;
        }
        case 0x62: // BIT 4, D
        {
            bit(mRegisters.getD(), 4);
            break;
        }
        case 0x63: // BIT 4, E
        {
            bit(mRegisters.getE(), 4);
            break;
        }
        case 0x64: // BIT 4, H
        {
            bit(mRegisters.getH(), 4);
            break;
        }
        case 0x65: // BIT 4, L
        {
            bit(mRegisters.getL(), 4);
            break;
        }
        case 0x66: // BIT 4, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 4);
            break;
        }
        case 0x67: // BIT 4, A
        {
            bit(mRegisters.getA(), 4);
            break;
        }
        case 0x68: // BIT 5, B
        {
            bit(mRegisters.getB(), 5);
            break;
        }
        case 0x69: // BIT 5, C
        {
            bit(mRegisters.getC(), 5);
            break;
        }
        case 0x6a: // BIT 5, D
        {
            bit(mRegisters.getD(), 5);
            break;
        }
        case 0x6b: // BIT 5, E
        {
            bit(mRegisters.getE(), 5);
            break;
        }
        case 0x6c: // BIT 5, H
        {
            bit(mRegisters.getH(), 5);
            break;
        }
        case 0x6d: // BIT 5, L
        {
            bit(mRegisters.getL(), 5);
            break;
        }
        case 0x6e: // BIT 5, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 5);
            break;
        }
        case 0x6f: // BIT 5, A
        {
            bit(mRegisters.getA(), 5);
            break;
        }
        case 0x70: // BIT 6, B
        {
            bit(mRegisters.getB(), 6);
            break;
        }
        case 0x71: // BIT 6, C
        {
            bit(mRegisters.getC(), 6);
            break;
        }
        case 0x72: // BIT 6, D
        {
            bit(mRegisters.getD(), 6);
            break;
        }
        case 0x73: // BIT 6, E
        {
            bit(mRegisters.getE(), 6);
            break;
        }
        case 0x74: // BIT 6, H
        {
            bit(mRegisters.getH(), 6);
            break;
        }
        case 0x75: // BIT 6, L
        {
            bit(mRegisters.getL(), 6);
            break;
        }
        case 0x76: // BIT 6, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 6);
            break;
        }
        case 0x77: // BIT 6, A
        {
            bit(mRegisters.getA(), 6);
            break;
        }
        case 0x78: // BIT 7, B
        {
            bit(mRegisters.getB(), 7);
            break;
        }
        case 0x79: // BIT 7, C
        {
            bit(mRegisters.getC(), 7);
            break;
        }
        case 0x7a: // BIT 7, D
        {
            bit(mRegisters.getD(), 7);
            break;
        }
        case 0x7b: // BIT 7, E
        {
            bit(mRegisters.getE(), 7);
            break;
        }
        case 0x7c: // BIT 7, H
        {
            bit(mRegisters.getH(), 7);
            break;
        }
        case 0x7d: // BIT 7, L
        {
            bit(mRegisters.getL(), 7);
            break;
        }
        case 0x7e: // BIT 7, (HL)
        {
            bit(bus.read(mRegisters.getHL()), 7);
            break;
        }
        case 0x7f: // BIT 7, A
        {
            bit(mRegisters.getA(), 7);
            break;
        }
        case 0x80: // RES 0, B
        {
            mRegisters.setB(res(mRegisters.getB(), 0));
            break;
        }
        case 0x81: // RES 0, C
        {
            mRegisters.setC(res(mRegisters.getC(), 0));
            break;
        }
        case 0x82: // RES 0, D
        {
            mRegisters.setD(res(mRegisters.getD(), 0));
            break;
        }
        case 0x83: // RES 0, E
        {
            mRegisters.setE(res(mRegisters.getE(), 0));
            break;
        }
        case 0x84: // RES 0, H
        {
            mRegisters.setH(res(mRegisters.getH(), 0));
            break;
        }
        case 0x85: // RES 0, L
        {
            mRegisters.setL(res(mRegisters.getL(), 0));
            break;
        }
        case 0x86: // RES 0, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 0));
            break;
        }
        case 0x87: // RES 0, A
        {
            mRegisters.setA(res(mRegisters.getA(), 0));
            break;
        }
        case 0x88: // RES 1, B
        {
            mRegisters.setB(res(mRegisters.getB(), 1));
            break;
        }
        case 0x89: // RES 1, C
        {
            mRegisters.setC(res(mRegisters.getC(), 1));
            break;
        }
        case 0x8a: // RES 1, D
        {
            mRegisters.setD(res(mRegisters.getD(), 1));
            break;
        }
        case 0x8b: // RES 1, E
        {
            mRegisters.setE(res(mRegisters.getE(), 1));
            break;
        }
        case 0x8c: // RES 1, H
        {
            mRegisters.setH(res(mRegisters.getH(), 1));
            break;
        }
        case 0x8d: // RES 1, L
        {
            mRegisters.setL(res(mRegisters.getL(), 1));
            break;
        }
        case 0x8e: // RES 1, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 1));
            break;
        }
        case 0x8f: // RES 1, A
        {
            mRegisters.setA(res(mRegisters.getA(), 1));
            break;
        }
        case 0x90: // RES 2, B
        {
            mRegisters.setB(res(mRegisters.getB(), 2));
            break;
        }
        case 0x91: // RES 2, C
        {
            mRegisters.setC(res(mRegisters.getC(), 2));
            break;
        }
        case 0x92: // RES 2, D
        {
            mRegisters.setD(res(mRegisters.getD(), 2));
            break;
        }
        case 0x93: // RES 2, E
        {
            mRegisters.setE(res(mRegisters.getE(), 2));
            break;
        }
        case 0x94: // RES 2, H
        {
            mRegisters.setH(res(mRegisters.getH(), 2));
            break;
        }
        case 0x95: // RES 2, L
        {
            mRegisters.setL(res(mRegisters.getL(), 2));
            break;
        }
        case 0x96: // RES 2, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 2));
            break;
        }
        case 0x97: // RES 2, A
        {
            mRegisters.setA(res(mRegisters.getA(), 2));
            break;
        }
        case 0x98: // RES 3, B
        {
            mRegisters.setB(res(mRegisters.getB(), 3));
            break;
        }
        case 0x99: // RES 3, C
        {
            mRegisters.setC(res(mRegisters.getC(), 3));
            break;
        }
        case 0x9a: // RES 3, D
        {
            mRegisters.setD(res(mRegisters.getD(), 3));
            break;
        }
        case 0x9b: // RES 3, E
        {
            mRegisters.setE(res(mRegisters.getE(), 3));
            break;
        }
        case 0x9c: // RES 3, H
        {
            mRegisters.setH(res(mRegisters.getH(), 3));
            break;
        }
        case 0x9d: // RES 3, L
        {
            mRegisters.setL(res(mRegisters.getL(), 3));
            break;
        }
        case 0x9e: // RES 3, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 3));
            break;
        }
        case 0x9f: // RES 3, A
        {
            mRegisters.setA(res(mRegisters.getA(), 3));
            break;
        }
        case 0xa0: // RES 4, B
        {
            mRegisters.setB(res(mRegisters.getB(), 4));
            break;
        }
        case 0xa1: // RES 4, C
        {
            mRegisters.setC(res(mRegisters.getC(), 4));
            break;
        }
        case 0xa2: // RES 4, D
        {
            mRegisters.setD(res(mRegisters.getD(), 4));
            break;
        }
        case 0xa3: // RES 4, E
        {
            mRegisters.setE(res(mRegisters.getE(), 4));
            break;
        }
        case 0xa4: // RES 4, H
        {
            mRegisters.setH(res(mRegisters.getH(), 4));
            break;
        }
        case 0xa5: // RES 4, L
        {
            mRegisters.setL(res(mRegisters.getL(), 4));
            break;
        }
        case 0xa6: // RES 4, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 4));
            break;
        }
        case 0xa7: // RES 4, A
        {
            mRegisters.setA(res(mRegisters.getA(), 4));
            break;
        }
        case 0xa8: // RES 5, B
        {
            mRegisters.setB(res(mRegisters.getB(), 5));
            break;
        }
        case 0xa9: // RES 5, C
        {
            mRegisters.setC(res(mRegisters.getC(), 5));
            break;
        }
        case 0xaa: // RES 5, D
        {
            mRegisters.setD(res(mRegisters.getD(), 5));
            break;
        }
        case 0xab: // RES 5, E
        {
            mRegisters.setE(res(mRegisters.getE(), 5));
            break;
        }
        case 0xac: // RES 5, H
        {
            mRegisters.setH(res(mRegisters.getH(), 5));
            break;
        }
        case 0xad: // RES 5, L
        {
            mRegisters.setL(res(mRegisters.getL(), 5));
            break;
        }
        case 0xae: // RES 5, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 5));
            break;
        }
        case 0xaf: // RES 5, A
        {
            mRegisters.setA(res(mRegisters.getA(), 5));
            break;
        }
        case 0xb0: // RES 6, B
        {
            mRegisters.setB(res(mRegisters.getB(), 6));
            break;
        }
        case 0xb1: // RES 6, C
        {
            mRegisters.setC(res(mRegisters.getC(), 6));
            break;
        }
        case 0xb2: // RES 6, D
        {
            mRegisters.setD(res(mRegisters.getD(), 6));
            break;
        }
        case 0xb3: // RES 6, E
        {
            mRegisters.setE(res(mRegisters.getE(), 6));
            break;
        }
        case 0xb4: // RES 6, H
        {
            mRegisters.setH(res(mRegisters.getH(), 6));
            break;
        }
        case 0xb5: // RES 6, L
        {
            mRegisters.setL(res(mRegisters.getL(), 6));
            break;
        }
        case 0xb6: // RES 6, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 6));
            break;
        }
        case 0xb7: // RES 6, A
        {
            mRegisters.setA(res(mRegisters.getA(), 6));
            break;
        }
        case 0xb8: // RES 7, B
        {
            mRegisters.setB(res(mRegisters.getB(), 7));
            break;
        }
        case 0xb9: // RES 7, C
        {
            mRegisters.setC(res(mRegisters.getC(), 7));
            break;
        }
        case 0xba: // RES 7, D
        {
            mRegisters.setD(res(mRegisters.getD(), 7));
            break;
        }
        case 0xbb: // RES 7, E
        {
            mRegisters.setE(res(mRegisters.getE(), 7));
            break;
        }
        case 0xbc: // RES 7, H
        {
            mRegisters.setH(res(mRegisters.getH(), 7));
            break;
        }
        case 0xbd: // RES 7, L
        {
            mRegisters.setL(res(mRegisters.getL(), 7));
            break;
        }
        case 0xbe: // RES 7, (HL)
        {
            bus.write(mRegisters.getHL(), res(bus.read(mRegisters.getHL()), 7));
            break;
        }
        case 0xbf: // RES 7, A
        {
            mRegisters.setA(res(mRegisters.getA(), 7));
            break;
        }
        case 0xc0: // SET 0, B
        {
            mRegisters.setB(set(mRegisters.getB(), 0));
            break;
        }
        case 0xc1: // SET 0, C
        {
            mRegisters.setC(set(mRegisters.getC(), 0));
            break;
        }
        case 0xc2: // SET 0, D
        {
            mRegisters.setD(set(mRegisters.getD(), 0));
            break;
        }
        case 0xc3: // SET 0, E
        {
            mRegisters.setE(set(mRegisters.getE(), 0));
            break;
        }
        case 0xc4: // SET 0, H
        {
            mRegisters.setH(set(mRegisters.getH(), 0));
            break;
        }
        case 0xc5: // SET 0, L
        {
            mRegisters.setL(set(mRegisters.getL(), 0));
            break;
        }
        case 0xc6: // SET 0, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 0));
            break;
        }
        case 0xc7: // SET 0, A
        {
            mRegisters.setA(set(mRegisters.getA(), 0));
            break;
        }
        case 0xc8: // SET 1, B
        {
            mRegisters.setB(set(mRegisters.getB(), 1));
            break;
        }
        case 0xc9: // SET 1, C
        {
            mRegisters.setC(set(mRegisters.getC(), 1));
            break;
        }
        case 0xca: // SET 1, D
        {
            mRegisters.setD(set(mRegisters.getD(), 1));
            break;
        }
        case 0xcb: // SET 1, E
        {
            mRegisters.setE(set(mRegisters.getE(), 1));
            break;
        }
        case 0xcc: // SET 1, H
        {
            mRegisters.setH(set(mRegisters.getH(), 1));
            break;
        }
        case 0xcd: // SET 1, L
        {
            mRegisters.setL(set(mRegisters.getL(), 1));
            break;
        }
        case 0xce: // SET 1, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 1));
            break;
        }
        case 0xcf: // SET 1, A
        {
            mRegisters.setA(set(mRegisters.getA(), 1));
            break;
        }
        case 0xd0: // SET 2, B
        {
            mRegisters.setB(set(mRegisters.getB(), 2));
            break;
        }
        case 0xd1: // SET 2, C
        {
            mRegisters.setC(set(mRegisters.getC(), 2));
            break;
        }
        case 0xd2: // SET 2, D
        {
            mRegisters.setD(set(mRegisters.getD(), 2));
            break;
        }
        case 0xd3: // SET 2, E
        {
            mRegisters.setE(set(mRegisters.getE(), 2));
            break;
        }
        case 0xd4: // SET 2, H
        {
            mRegisters.setH(set(mRegisters.getH(), 2));
            break;
        }
        case 0xd5: // SET 2, L
        {
            mRegisters.setL(set(mRegisters.getL(), 2));
            break;
        }
        case 0xd6: // SET 2, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 2));
            break;
        }
        case 0xd7: // SET 2, A
        {
            mRegisters.setA(set(mRegisters.getA(), 2));
            break;
        }
        case 0xd8: // SET 3, B
        {
            mRegisters.setB(set(mRegisters.getB(), 3));
            break;
        }
        case 0xd9: // SET 3, C
        {
            mRegisters.setC(set(mRegisters.getC(), 3));
            break;
        }
        case 0xda: // SET 3, D
        {
            mRegisters.setD(set(mRegisters.getD(), 3));
            break;
        }
        case 0xdb: // SET 3, E
        {
            mRegisters.setE(set(mRegisters.getE(), 3));
            break;
        }
        case 0xdc: // SET 3, H
        {
            mRegisters.setH(set(mRegisters.getH(), 3));
            break;
        }
        case 0xdd: // SET 3, L
        {
            mRegisters.setL(set(mRegisters.getL(), 3));
            break;
        }
        case 0xde: // SET 3, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 3));
            break;
        }
        case 0xdf: // SET 3, A
        {
            mRegisters.setA(set(mRegisters.getA(), 3));
            break;
        }
        case 0xe0: // SET 4, B
        {
            mRegisters.setB(set(mRegisters.getB(), 4));
            break;
        }
        case 0xe1: // SET 4, C
        {
            mRegisters.setC(set(mRegisters.getC(), 4));
            break;
        }
        case 0xe2: // SET 4, D
        {
            mRegisters.setD(set(mRegisters.getD(), 4));
            break;
        }
        case 0xe3: // SET 4, E
        {
            mRegisters.setE(set(mRegisters.getE(), 4));
            break;
        }
        case 0xe4: // SET 4, H
        {
            mRegisters.setH(set(mRegisters.getH(), 4));
            break;
        }
        case 0xe5: // SET 4, L
        {
            mRegisters.setL(set(mRegisters.getL(), 4));
            break;
        }
        case 0xe6: // SET 4, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 4));
            break;
        }
        case 0xe7: // SET 4, A
        {
            mRegisters.setA(set(mRegisters.getA(), 4));
            break;
        }
        case 0xe8: // SET 5, B
        {
            mRegisters.setB(set(mRegisters.getB(), 5));
            break;
        }
        case 0xe9: // SET 5, C
        {
            mRegisters.setC(set(mRegisters.getC(), 5));
            break;
        }
        case 0xea: // SET 5, D
        {
            mRegisters.setD(set(mRegisters.getD(), 5));
            break;
        }
        case 0xeb: // SET 5, E
        {
            mRegisters.setE(set(mRegisters.getE(), 5));
            break;
        }
        case 0xec: // SET 5, H
        {
            mRegisters.setH(set(mRegisters.getH(), 5));
            break;
        }
        case 0xed: // SET 5, L
        {
            mRegisters.setL(set(mRegisters.getL(), 5));
            break;
        }
        case 0xee: // SET 5, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 5));
            break;
        }
        case 0xef: // SET 5, A
        {
            mRegisters.setA(set(mRegisters.getA(), 5));
            break;
        }
        case 0xf0: // SET 6, B
        {
            mRegisters.setB(set(mRegisters.getB(), 6));
            break;
        }
        case 0xf1: // SET 6, C
        {
            mRegisters.setC(set(mRegisters.getC(), 6));
            break;
        }
        case 0xf2: // SET 6, D
        {
            mRegisters.setD(set(mRegisters.getD(), 6));
            break;
        }
        case 0xf3: // SET 6, E
        {
            mRegisters.setE(set(mRegisters.getE(), 6));
            break;
        }
        case 0xf4: // SET 6, H
        {
            mRegisters.setH(set(mRegisters.getH(), 6));
            break;
        }
        case 0xf5: // SET 6, L
        {
            mRegisters.setL(set(mRegisters.getL(), 6));
            break;
        }
        case 0xf6: // SET 6, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 6));
            break;
        }
        case 0xf7: // SET 6, A
        {
            mRegisters.setA(set(mRegisters.getA(), 6));
            break;
        }
        case 0xf8: // SET 7, B
        {
            mRegisters.setB(set(mRegisters.getB(), 7));
            break;
        }
        case 0xf9: // SET 7, C
        {
            mRegisters.setC(set(mRegisters.getC(), 7));
            break;
        }
        case 0xfa: // SET 7, D
        {
            mRegisters.setD(set(mRegisters.getD(), 7));
            break;
        }
        case 0xfb: // SET 7, E
        {
            mRegisters.setE(set(mRegisters.getE(), 7));
            break;
        }
        case 0xfc: // SET 7, H
        {
            mRegisters.setH(set(mRegisters.getH(), 7));
            break;
        }
        case 0xfd: // SET 7, L
        {
            mRegisters.setL(set(mRegisters.getL(), 7));
            break;
        }
        case 0xfe: // SET 7, (HL)
        {
            bus.write(mRegisters.getHL(), set(bus.read(mRegisters.getHL()), 7));
            break;
        }
        case 0xff: // SET 7, A
        {
            mRegisters.setA(set(mRegisters.getA(), 7));
            break;
        }
        }
        break;
    }
    default:
        throw std::runtime_error(std::format("{} is not supported!", opcode));
    }

    if (mIMEEnableScheduled)
    {
        mIME = true;
        mIMEEnableScheduled = false;
    }
    else if (mIMEEnablePending)
    {
        mIMEEnableScheduled = true;
        mIMEEnablePending = false;
    }
}

uint8_t CPU::fetchByte(Bus &bus)
{
    uint8_t byte = bus.read(mRegisters.getPC());
    mRegisters.setPC(mRegisters.getPC() + 1);
    return byte;
}

uint16_t CPU::fetchWord(Bus &bus)
{
    uint8_t low = fetchByte(bus);
    uint8_t high = fetchByte(bus);
    return makeWord(low, high);
}

uint8_t CPU::increment(uint8_t r8)
{
    uint8_t res = r8 + 1;
    bool halfCarryFlag = (r8 & 0x0f) + 0x01 > 0x0f;

    mRegisters.setFlag(Flag::Zero, res == 0);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, halfCarryFlag);

    return res;
}

uint8_t CPU::decrement(uint8_t r8)
{
    uint8_t res = r8 - 1;
    bool halfCarryFlag = (r8 & 0x0f) == 0;

    mRegisters.setFlag(Flag::Zero, res == 0);
    mRegisters.setFlag(Flag::Subtraction, true);
    mRegisters.setFlag(Flag::HalfCarry, halfCarryFlag);

    return res;
}

uint16_t CPU::add(uint16_t first, uint16_t second)
{
    uint32_t res = first + second;

    bool setHalfCarry = (first & 0x0fff) + (second & 0x0fff) > 0x0fff;
    bool setCarry = res > 0xffff;
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, setHalfCarry);
    mRegisters.setFlag(Flag::Carry, setCarry);

    return static_cast<uint16_t>(res);
}

uint8_t CPU::add(uint8_t first, uint8_t second)
{
    uint16_t res = first + second;

    bool setHalfCarry = (first & 0x0f) + (second & 0x0f) > 0x0f;
    bool setCarry = res > 0xff;
    mRegisters.setFlag(Flag::Zero, static_cast<uint8_t>(res) == 0);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, setHalfCarry);
    mRegisters.setFlag(Flag::Carry, setCarry);

    return static_cast<uint8_t>(res);
}

uint8_t CPU::addCarry(uint8_t first, uint8_t second)
{
    uint8_t carry = mRegisters.getFlag(Flag::Carry) ? 1 : 0;
    uint16_t res = first + second + carry;

    bool setHalfCarry = (first & 0x0f) + (second & 0x0f) + carry > 0x0f;
    bool setCarry = res > 0xff;
    mRegisters.setFlag(Flag::Zero, static_cast<uint8_t>(res) == 0);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, setHalfCarry);
    mRegisters.setFlag(Flag::Carry, setCarry);

    return static_cast<uint8_t>(res);
}

uint8_t CPU::sub(uint8_t first, uint8_t second)
{
    uint16_t res = first - second;

    bool setHalfCarry = (first & 0x0f) < (second & 0x0f);
    bool setCarry = res > 0xff;
    mRegisters.setFlag(Flag::Zero, static_cast<uint8_t>(res) == 0);
    mRegisters.setFlag(Flag::Subtraction, true);
    mRegisters.setFlag(Flag::HalfCarry, setHalfCarry);
    mRegisters.setFlag(Flag::Carry, setCarry);

    return static_cast<uint8_t>(res);
}

uint8_t CPU::subCarry(uint8_t first, uint8_t second)
{
    uint8_t carry = mRegisters.getFlag(Flag::Carry);
    uint16_t res = first - second - carry;

    bool setHalfCarry = (first & 0x0f) - (second & 0x0f) - carry < 0;
    bool setCarry = res > 0xff;

    mRegisters.setFlag(Flag::Zero, static_cast<uint8_t>(res) == 0);
    mRegisters.setFlag(Flag::Subtraction, true);
    mRegisters.setFlag(Flag::HalfCarry, setHalfCarry);
    mRegisters.setFlag(Flag::Carry, setCarry);

    return static_cast<uint8_t>(res);
}

uint8_t CPU::opAnd(uint8_t first, uint8_t second)
{
    uint8_t res = first & second;

    mRegisters.setFlag(Flag::Zero, res == 0);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, true);
    mRegisters.setFlag(Flag::Carry, false);

    return res;
}

uint8_t CPU::opXor(uint8_t first, uint8_t second)
{
    uint8_t res = first ^ second;

    mRegisters.setFlag(Flag::Zero, res == 0);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, false);

    return res;
}

uint8_t CPU::opOr(uint8_t first, uint8_t second)
{
    uint8_t res = first | second;

    mRegisters.setFlag(Flag::Zero, res == 0);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, false);

    return res;
}

void CPU::cp(uint8_t first, uint8_t second)
{
    uint8_t res = first - second;

    bool setHalfCarry = (first & 0x0f) < (second & 0x0f);
    bool setCarry = first < second;

    mRegisters.setFlag(Flag::Zero, res == 0);
    mRegisters.setFlag(Flag::Subtraction, true);
    mRegisters.setFlag(Flag::HalfCarry, setHalfCarry);
    mRegisters.setFlag(Flag::Carry, setCarry);
}

uint16_t CPU::pop16(Bus &bus)
{
    uint16_t sp = mRegisters.getSP();
    uint8_t low = bus.read(sp++);
    uint8_t high = bus.read(sp++);
    mRegisters.setSP(sp);
    return makeWord(high, low);
}

void CPU::push16(Bus &bus, uint16_t value)
{
    uint16_t sp = mRegisters.getSP();
    auto [high, low] = makeBytes(value);
    bus.write(--sp, high);
    bus.write(--sp, low);
    mRegisters.setSP(sp);
}

uint8_t CPU::rlc(uint8_t reg)
{
    uint8_t bit7 = (reg >> 7) & 0x01;
    reg = (reg << 1) | bit7;

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, 1 == bit7);

    return reg;
}

uint8_t CPU::rl(uint8_t reg)
{
    uint8_t bit7 = (reg >> 7) & 0x01;
    uint8_t oldCarry = mRegisters.getFlag(Flag::Carry) ? 1 : 0;

    reg = (reg << 1) | oldCarry;

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, 1 == bit7);

    return reg;
}

uint8_t CPU::rrc(uint8_t reg)
{
    uint8_t bit0 = reg & 0x01;
    reg = (reg >> 1) | (bit0 << 7);

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, 1 == bit0);

    return reg;
}

uint8_t CPU::rr(uint8_t reg)
{
    uint8_t bit0 = reg & 0x01;
    uint8_t oldCarry = mRegisters.getFlag(Flag::Carry) ? 1 : 0;

    reg = (reg >> 1) | (oldCarry << 7);

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, 1 == bit0);

    return reg;
}

uint8_t CPU::sla(uint8_t reg)
{
    uint8_t bit7 = (reg >> 7) & 0x01;
    reg = reg << 1;

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, 1 == bit7);

    return reg;
}

uint8_t CPU::sra(uint8_t reg)
{
    uint8_t bit0 = reg & 0x01;
    uint8_t bit7 = reg & 0x80; // Keep original bit 7
    reg = (reg >> 1) | bit7;   // Shift right and restore bit 7

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, 1 == bit0);

    return reg;
}

uint8_t CPU::swap(uint8_t reg)
{
    reg = (reg << 4) | (reg >> 4);

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, false);

    return reg;
}

uint8_t CPU::srl(uint8_t reg)
{
    uint8_t bit0 = reg & 1;
    reg >>= 1;

    mRegisters.setFlag(Flag::Zero, 0 == reg);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, false);
    mRegisters.setFlag(Flag::Carry, 1 == bit0);

    return reg;
}

void CPU::bit(uint8_t reg, uint8_t bitIndex)
{
    bool isSet = (reg >> bitIndex) & 1;
    mRegisters.setFlag(Flag::Zero, !isSet);
    mRegisters.setFlag(Flag::Subtraction, false);
    mRegisters.setFlag(Flag::HalfCarry, true);
}

uint8_t CPU::res(uint8_t reg, uint8_t bitIndex)
{
    return reg & ~(1 << bitIndex);
}

uint8_t CPU::set(uint8_t reg, uint8_t bitIndex)
{
    return reg | (1 << bitIndex);
}
} // namespace qgb