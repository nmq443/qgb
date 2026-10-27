#pragma once

#include <cstdint>
#include <filesystem>
#include <type_traits>
#include <vector>

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
 * @brief Get status of a bit of a value
 * @param num Target number
 * @param flag Target bit (pass enum class in)
 * @return Status of the bit
 */
template <typename Enum>
    requires std::is_enum_v<Enum>
auto getBit(uint8_t num, Enum flag) -> bool
{
    using Underlying = std::underlying_type_t<Enum>;
    return ((num >> static_cast<Underlying>(flag)) & 1) > 0;
}

/**
 * @brief Set status of a bit of a value
 * @param num Target number
 * @param flag Target bit (pass enum class in)
 * @param bit Status on or off
 * @return Value after setting bit status
 */
template <typename Enum>
    requires std::is_enum_v<Enum>
auto setBit(uint8_t num, Enum flag, bool status) -> uint8_t
{
    using Underlying = std::underlying_type_t<Enum>;
    auto bit = static_cast<Underlying>(flag);
    if (status)
    {
        num |= 1 << bit;
    }
    else
    {
        num &= ~(1 << bit);
    }
    return num;
}

/**
 * @brief Get ROM from a binary file
 * @param filepath
 * @return
 */
std::vector<uint8_t> readGbRom(const std::filesystem::path &filepath);
} // namespace qgb