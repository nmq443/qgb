#pragma once

#include <cstdint>
#include <filesystem>
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
 * @param bit Target bit
 * @return Status of the bit
 */
auto getBit(uint8_t num, uint8_t bit) -> bool;

/**
 * @brief Set status of a bit of a value
 * @param num Target number
 * @param bit Target bit
 * @param bit Status on or off
 * @return Value after setting bit status
 */
auto setBit(uint8_t num, uint8_t bit, bool status) -> uint8_t;

/**
 * @brief Get ROM from a binary file
 * @param filepath
 * @return
 */
std::vector<uint8_t> readGbRom(const std::filesystem::path &filepath);
} // namespace qgb