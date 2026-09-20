#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

namespace qgb
{
    /**
     * @brief Get 16-bit unsigned word from high and low byte
     * @param high High byte
     * @param low Low byte
     * @return 
     */
    uint16_t makeWord(uint8_t high, uint8_t low);

    /**
     * @brief Get high and low byte from a 16-bit unsigned word
     * @param word 
     * @return High, low byte
     */
    std::pair<uint8_t, uint8_t> makeBytes(uint16_t word);

    /**
     * @brief Get ROM from a binary file
     * @param filepath 
     * @return 
     */
    std::vector<uint8_t> readGbRom(const std::filesystem::path& filepath);
}