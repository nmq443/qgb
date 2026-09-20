#include "Utils.h"
#include <fstream>

namespace qgb
{
    uint16_t makeWord(uint8_t high, uint8_t low)
    {
        return high << 8 | low;
    }

    std::pair<uint8_t, uint8_t> makeBytes(uint16_t word)
    {
        return std::make_pair(static_cast<uint8_t>(word >> 8), static_cast<uint8_t>(word & 0xff));
    }

    std::vector<uint8_t> readGbRom(const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filepath.string());
        }

        auto size = std::filesystem::file_size(filepath);
        std::vector<uint8_t> rom(size);

        if (!file.read(reinterpret_cast<char*>(rom.data()), size))
        {
            throw std::runtime_error("Failed to read file: " + filepath.string());
        }

        return rom;
    }
}