#include "APU.h"
#include "Utils.h"

namespace qgb
{
auto AudioRing::available() const -> size_t
{
    return writePos.load(std::memory_order_acquire) - readPos.load(std::memory_order_acquire);
}

auto AudioRing::push(float l, float r) -> bool
{
    size_t currentWritePos = writePos.load(std::memory_order_relaxed); // emulator thread control writePos (via push())
    size_t currentReadPos = readPos.load(std::memory_order_acquire);   // audio thread control readPos (via pop())

    if (currentWritePos - currentReadPos == kCapacity)
    {
        return false; // full
    }

    size_t i = (currentWritePos & kMask) * 2; // the same as (currentWritePos % kCapacity) * 2, but using bitwise operation)
    data[i] = l;
    data[i + 1] = r;

    writePos.store(currentWritePos + 1, std::memory_order_release);
    return true;
}

auto AudioRing::pop(float *out, size_t frames) -> size_t
{
    size_t currentReadPos = readPos.load(std::memory_order_relaxed);   // audio thread control readPos (via pop())
    size_t currentWritePos = writePos.load(std::memory_order_acquire); // emulator thread control writePos (via push())

    size_t avail = currentWritePos - currentReadPos;
    size_t n = frames < avail ? frames : avail;

    for (size_t k = 0; k < n; ++k)
    {
        size_t i = ((currentReadPos + k) & kMask) * 2; // the same as (((currentReadPos + k) % kCapacity) * 2, but using bitwise operation)
        out[k * 2] = data[i];
        out[k * 2 + 1] = data[i + 1];
    }

    readPos.store(currentReadPos + n, std::memory_order_release);
    return n;
}

void APU::tick(int tCycles)
{
    mSampleTimer += tCycles;

    if (getBit(mRegs[kNR52 - kBaseAddr], AudioMasterControl::AudioOn))
    {
        mFrameSeqCounter += tCycles;
        while (mFrameSeqCounter >= kFrameSeqPeriod)
        {
            mFrameSeqCounter -= kFrameSeqPeriod;
            stepFrameSequencer();
        }
    }

    while (mSampleTimer >= kCyclesPerSample)
    {
        mSampleTimer -= kCyclesPerSample;
        pushSample();
    }
}

auto APU::read(uint16_t address) const -> uint8_t
{
    if (0xff30 <= address and address <= 0xff3f)
    {
        return mWaveRam[address - 0xff30];
    }
    if (address < 0xff10 or address > 0xff2f)
    {
        return 0xff;
    }
    if (address > 0xff26) // unused 0xff27-0xff2f
    {
        return 0xff;
    }
    auto value = mRegs[address - 0xff10];
    if (kNR52 == address)
    {
        value = (value & 0b1000'0000) | 0b0111'0000;
        for (size_t i = 0; i < 4; ++i)
        {
            if (mChannels[i].enabled)
            {
                value |= (1 << i);
            }
        }
        return value;
    }
    return value | kReadMask[address - 0xff10];
}

void APU::write(uint16_t address, uint8_t value)
{
    if (0xff30 <= address and address <= 0xff3f)
    {
        mWaveRam[address - 0xff30] = value;
        return;
    }
    if (address < 0xff10 or address > 0xff26)
    {
        return;
    }

    bool wasOn = getBit(mRegs[kNR52 - 0xff10], AudioMasterControl::AudioOn);
    bool nowOn = getBit(value, AudioMasterControl::AudioOn);
    if (!wasOn and nowOn) // power on reset the step and counter
    {
        mFrameSeqCounter = 0;
        mFrameSeqStep = 0;
    }
    if (kNR52 == address)
    {
        if (wasOn and !nowOn)
        {
            powerOff();
        }
        mRegs[kNR52 - 0xff10] = value & 0b1000'0000;
        return;
    }

    // DMG: only length registers still writable
    uint16_t length = 64 - (value & 0x3f);
    switch (address)
    {
    case kNR11:
        mChannels[0].length = length;
        break;
    case kNR21:
        mChannels[1].length = length;
        break;
    case kNR41:
        mChannels[3].length = length;
        break;
    case kNR31:
        mChannels[2].length = 256 - value;
        break;
    default:
        break;
    }

    if (wasOn)
    {
        mRegs[address - kBaseAddr] = value;
    }
}

void APU::powerOff()
{
    auto nr52 = mRegs[kNR52 - 0xff10];
    mRegs.fill(0); // clear NR10-NR51
    mRegs[kNR52 - 0xff10] = nr52;
    for (auto &channel : mChannels)
    {
        channel.enabled = false;
    }
}

auto APU::getChannel(uint8_t index) const -> const Channel &
{
    return mChannels[index];
}

float testSquareWave()
{
    constexpr double kFreq = 440.0;
    static double phase = 0.0;

    float s = (phase < 0.5) ? 0.25f : -0.25f;
    phase += kFreq / 48000;
    if (phase >= 1.0)
    {
        phase -= 1.0;
    }
    return s;
}

float APU::ch1Output() { return testSquareWave(); } // your hardcoded wave
float APU::ch2Output() { return 0.0f; }
float APU::ch3Output() { return 0.0f; }
float APU::ch4Output() { return 0.0f; }

void APU::pushSample()
{
    // Each channel returns a float in [-1, 1] (0.0 if disabled)
    const float ch[4] = {ch1Output(), ch2Output(), ch3Output(), ch4Output()};

    float left = 0.f;
    float right = 0.f;

    uint8_t nr51 = mRegs[kNR51 - kBaseAddr];
    for (size_t i = 0; i < mChannels.size(); ++i)
    {
        if (getBit(nr51, static_cast<int>(SoundPanning::CH1Left) + i))
        {
            left += ch[i];
        }
        if (getBit(nr51, static_cast<int>(SoundPanning::CH1Right) + i))
        {
            right += ch[i];
        }
    }

    // Average of 4 channels keeps the sum inside [-1, 1]
    left /= static_cast<float>(mChannels.size());
    right /= static_cast<float>(mChannels.size());

    float leftVol = (((mRegs[kNR50 - kBaseAddr] >> 4) & 0b0000'0111) + 1) / 8.f;
    float rightVol = ((mRegs[kNR50 - kBaseAddr] & 0b0000'0111) + 1) / 8.f;

    left *= leftVol;
    right *= rightVol;

    if (!getBit(mRegs[kNR52 - kBaseAddr], AudioMasterControl::AudioOn))
    {
        left = right = 0.f;
    }

    mRing.push(left, right);
}

auto APU::getRingBuffer() -> AudioRing &
{
    return mRing;
}

void APU::stepFrameSequencer()
{
    switch (mFrameSeqStep)
    {
    case 0:
    case 4:
        clockLength();
        break;
    case 2:
    case 6:
        clockLength();
        clockSweep();
        break;
    case 7:
        clockEnvelope();
        break;
    }
    mFrameSeqStep = (mFrameSeqStep + 1) & 7; // identical to (mFrameSeqStep + 1) % 8
}

void APU::clockLength() {}

void APU::clockSweep() {}

void APU::clockEnvelope() {}
} // namespace qgb
