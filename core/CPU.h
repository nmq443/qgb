#pragma once

#include "Bus.h"
#include "Clock.h"
#include "Registers.h"
#include <cstdint>

namespace qgb
{
class CPU
{
public:
    CPU() = default;
    int step();
    void init(Bus &bus, Clock &clock);
    ~CPU();

private:
    uint8_t fetch8();
    uint16_t fetch16();
    uint8_t increment(uint8_t r8);
    uint8_t decrement(uint8_t r8);
    uint16_t add(uint16_t first, uint16_t second);
    uint8_t add(uint8_t first, uint8_t second);
    uint8_t sub(uint8_t first, uint8_t second);
    uint8_t addCarry(uint8_t first, uint8_t second);
    uint8_t subCarry(uint8_t first, uint8_t second);
    uint8_t opAnd(uint8_t first, uint8_t second);
    uint8_t opXor(uint8_t first, uint8_t second);
    uint8_t opOr(uint8_t first, uint8_t second);
    void cp(uint8_t first, uint8_t second);
    uint16_t pop16();
    void push16(uint16_t value);
    uint8_t rlc(uint8_t reg);
    uint8_t rl(uint8_t reg);
    uint8_t rrc(uint8_t reg);
    uint8_t rr(uint8_t reg);
    uint8_t sla(uint8_t reg);
    uint8_t sra(uint8_t reg);
    uint8_t swap(uint8_t reg);
    uint8_t srl(uint8_t reg);
    void bit(uint8_t reg, uint8_t bitIndex);
    uint8_t res(uint8_t reg, uint8_t bitIndex);
    uint8_t set(uint8_t reg, uint8_t bitIndex);
    int handleInterrupt();

    // CPU primitives
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
    void idle();

    Clock *mClockPtr; // non owning
    Bus *mBusPtr;     // non owning
    Registers mRegisters;
    bool mIsStopped = false;
    bool mHalted = false;
    bool mIME = false;
    bool mIMEEnableScheduled = false;
    bool mIMEEnablePending = false;
};
} // namespace qgb
