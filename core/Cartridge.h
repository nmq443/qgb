#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace qgb
{
class Cartridge
{
public:
    Cartridge() = default;
    void load(const std::filesystem::path &path);
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
    std::vector<uint8_t> mRom;
};
} // namespace qgb
