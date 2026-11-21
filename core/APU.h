#pragma once

#include "IClockable.h"
#include <array>
#include <atomic>
#include <cstdint>

namespace qgb
{
enum class AudioMasterControl
{
    CH1On,
    CH2On,
    CH3On,
    CH4On,
    AudioOn = 7,
};

enum class SoundPanning
{
    CH1Right,
    CH2Right,
    CH3Right,
    CH4Right,
    CH1Left,
    CH2Left,
    CH3Left,
    CH4Left,
};

struct Channel
{
    bool enabled = false;
    uint16_t length = 0; // CH3 need up to 256
    int8_t sample = 0;
};

struct AudioRing
{
    static constexpr size_t kCapacity = 8192;
    static constexpr size_t kMask = kCapacity - 1;
    std::array<float, kCapacity * 2> data = {};
    std::atomic<size_t> readPos = 0;
    std::atomic<size_t> writePos = 0;

    auto available() const -> size_t;

    /**
     * @brief Emulator thread. Returns false on overrun (frame dropped).
     * @param l Left channel
     * @param r Right channel
     * @return Overrun or not
     */
    auto push(float l, float r) -> bool;

    /**
     * @brief Audio thread. Returns how many frames were actually copied.
     * @param out Buffer to write
     * @param frames Number of frames requested
     * @return Number of frames were actually copied
     */
    auto pop(float *out, size_t frames) -> size_t;
};

class APU : public IClockable
{
public:
    static constexpr uint16_t kNR10 = 0xff10;
    static constexpr uint16_t kNR11 = 0xff11;
    static constexpr uint16_t kNR12 = 0xff12;
    static constexpr uint16_t kNR13 = 0xff13;
    static constexpr uint16_t kNR14 = 0xff14;

    static constexpr uint16_t kNR21 = 0xff16;
    static constexpr uint16_t kNR22 = 0xff17;
    static constexpr uint16_t kNR23 = 0xff18;
    static constexpr uint16_t kNR24 = 0xff19;

    static constexpr uint16_t kNR30 = 0xff1a;
    static constexpr uint16_t kNR31 = 0xff1b;
    static constexpr uint16_t kNR32 = 0xff1c;
    static constexpr uint16_t kNR33 = 0xff1d;
    static constexpr uint16_t kNR34 = 0xff1e;

    static constexpr uint16_t kNR41 = 0xff20;
    static constexpr uint16_t kNR42 = 0xff21;
    static constexpr uint16_t kNR43 = 0xff22;
    static constexpr uint16_t kNR44 = 0xff23;

    static constexpr uint16_t kNR51 = 0xff25;
    static constexpr uint16_t kNR50 = 0xff24;
    static constexpr uint16_t kNR52 = 0xff26;

    static constexpr uint16_t kBaseAddr = 0xff10;

    static constexpr double kCyclesPerSample = 4194304.0 / 48000;

    static constexpr std::array<uint8_t, 0x17> kReadMask = {
        0x80, 0x3f, 0x00, 0xff, 0xbf, // NR10-14
        0xff, 0x3f, 0x00, 0xff, 0xbf, // 0xff15 (unused), NR21-24
        0x7f, 0xff, 0x9f, 0xff, 0xbf, // NR30-34
        0xff, 0xff, 0x00, 0x00, 0xbf, // 0xff1F (unused), NR41-44
        0x00, 0x00, 0x70              // NR50-52
    };

    APU() = default;

    void tick(int tCycles) override;
    auto read(uint16_t address) const -> uint8_t;
    void write(uint16_t address, uint8_t value);
    void powerOff();
    auto getChannel(uint8_t index) const -> const Channel &;
    void pushSample();
    auto getRingBuffer() -> AudioRing &;

    // temp
    float ch1Output();
    float ch2Output();
    float ch3Output();
    float ch4Output();

private:
    std::array<uint8_t, 23> mRegs{};
    std::array<uint8_t, 16> mWaveRam = {};
    std::array<Channel, 4> mChannels = {}; // CH1..4
    double mSampleTimer = 0.0;
    AudioRing mRing;
};
} // namespace qgb
