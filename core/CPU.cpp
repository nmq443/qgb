#include "CPU.h"
#include "Utils.h"
#include <cstdint>
#include <regex>

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
} // namespace qgb