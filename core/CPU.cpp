#include "CPU.h"
#include "Utils.h"

namespace qgb
{
    void CPU::step(Bus& bus)
    {
        uint8_t opcode = fetchByte(bus);
        switch (opcode)
        {
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
        }
    }

    uint8_t CPU::fetchByte(Bus& bus)
    {
        uint8_t byte = bus.read(mRegisters.getPC());
        mRegisters.setPC(mRegisters.getPC() + 1);
        return byte;
    }

    uint16_t CPU::fetchWord(Bus& bus)
    {
        uint8_t low = fetchByte(bus);
        uint8_t high = fetchByte(bus);
        return makeWord(low, high);
    }
}