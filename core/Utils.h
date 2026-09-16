#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

namespace qgb
{
    uint16_t makeWord(uint8_t low, uint8_t high);

    std::vector<uint8_t> readGbRom(const std::filesystem::path& filepath);
}