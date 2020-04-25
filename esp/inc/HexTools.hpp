/**
 * @brief Hex tools header
 * 
 * @file HexTools.hpp
 */

#ifndef HEXTOOLS_HPP
#define HEXTOOLS_HPP

/* Includes -------------------------------------------- */
#include <Arduino.h>

/* C system */
#include <stdint.h>

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Forward declarations -------------------------------- */

/* Hex tool functions ---------------------------------- */
/**
 * @brief Removes the "0x" prefix from a hex string
 * if there is one.
 * 
 * @param[in, out]  pHexStr     Hex string to change
 * 
 * @return Bool :
 * - true : the Hex string was changed
 * - false : the hex string did not change
 */
bool remove0xPrefix(String &pHexStr);

/**
 * @brief Adds the "0x" prefix from a hex string
 * if there isn't one.
 * 
 * @param[in, out]  pHexStr     Hex string to change
 * 
 * @return Bool :
 * - true : the Hex string was changed
 * - false : the hex string did not change
 */
bool add0xPrefix(String &pHexStr);

/**
 * @brief Converts a uint8_t value to a hexadecimal string.
 * The hexadecimal is filled with zeros to fit the type size.
 * 
 * @param[in]   pInt    Integer to convert
 * @param[in]   pZeroX  Weither we add the "0x" prefix or not
 * 
 * @return The resulting hexadecimal string
 */
String uint8ToHexStr(const uint8_t &pInt, const bool &pZeroX = true);

/**
 * @brief Converts a uint16_t value to a hexadecimal string.
 * The hexadecimal is filled with zeros to fit the type size.
 * 
 * @param[in]   pInt    Integer to convert
 * @param[in]   pZeroX  Weither we add the "0x" prefix or not
 * 
 * @return The resulting hexadecimal string
 */
String uint16ToHexStr(const uint16_t &pInt, const bool &pZeroX = true);

/**
 * @brief Converts a uint32_t value to a hexadecimal string.
 * The hexadecimal is filled with zeros to fit the type size.
 * 
 * @param[in]   pInt    Integer to convert
 * @param[in]   pZeroX  Weither we add the "0x" prefix or not
 * 
 * @return The resulting hexadecimal string
 */
String uint32ToHexStr(const uint32_t &pInt, const bool &pZeroX = true);

/**
 * @brief Converts a uint64_t value to a hexadecimal string.
 * The hexadecimal is filled with zeros to fit the type size.
 * 
 * @param[in]   pInt    Integer to convert
 * @param[in]   pZeroX  Weither we add the "0x" prefix or not
 * 
 * @return The resulting hexadecimal string
 */
String uint64ToHexStr(const uint64_t &pInt, const bool &pZeroX = true);

bool hexStrToUInt8(const String &pStr, uint8_t &pInt);
bool hexStrToUInt16(const String &pStr, uint16_t &pInt);

#endif /* HEXTOOLS_HPP */