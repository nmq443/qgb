#include "Cartridge.h"
#include "Utils.h"

namespace qgb
{
void Cartridge::load(const std::filesystem::path &path)
{
    mRom = readGbRom(path);
}

uint8_t Cartridge::read(uint16_t address) const
{
    return mRom[address];
}

void Cartridge::write(uint16_t, uint8_t)
{
    /* ignored for now, no MBC support yet */
}
} // namespace qgb