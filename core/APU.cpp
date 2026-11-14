#include "APU.h"
#include "Utils.h"

namespace qgb
{
void APU::tick(int)
{
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
    if (kNR52 == address)
    {
        bool nowOn = getBit(value, AudioMasterControl::AudioOn);
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
    default: // channel 3
        mChannels[2].length = 256 - value;
        break;
    }

    if (wasOn)
    {
        mRegs[address - 0xff10] = value;
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
} // namespace qgb
