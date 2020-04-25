/**
 * @brief Hex string tools implementation
 * 
 * @file HexTools.cpp
 */

/* Includes -------------------------------------------- */
#include "HexTools.hpp"

/* Arduino */
#include <Arduino.h>

/* C system */
#include <string.h> /* memset() */

/* Defines --------------------------------------------- */

/* Type definitions ------------------------------------ */

/* Variable declaration -------------------------------- */

/* Tool functions -------------------------------------- */
bool remove0xPrefix(String &pHexStr) {
    if(2 < pHexStr.length()) {
        pHexStr = pHexStr.substring(2U);
        return true;
    } else return false;
}

bool add0xPrefix(String &pHexStr) {
    pHexStr = "0x" + pHexStr;
    return true;
}

String uint8ToHexStr(const uint8_t &pInt, const bool &pZeroX) {
    char lChars[5U];
    memset(lChars, 0, 5U);
    if(pZeroX) {
        snprintf(lChars, 5U, "0x%02X", pInt);
    } else {
        snprintf(lChars, 3U, "%02X", pInt);
    }

    return String(lChars);
}

String uint16ToHexStr(const uint16_t &pInt, const bool &pZeroX) {
    char lChars[7U];
    memset(lChars, 0, 7U);
    if(pZeroX) {
        snprintf(lChars, 7U, "0x%04X", pInt);
    } else {
        snprintf(lChars, 5U, "%04X", pInt);
    }

    return String(lChars);
}

String uint32ToHexStr(const uint32_t &pInt, const bool &pZeroX) {
    char lChars[11U];
    memset(lChars, 0, 11U);
    if(pZeroX) {
        snprintf(lChars, 11U, "0x%08X", pInt);
    } else {
        snprintf(lChars, 9U, "%08X", pInt);
    }

    return String(lChars);
}

String uint64ToHexStr(const uint64_t &pInt, const bool &pZeroX) {
    char lChars[19U];
    memset(lChars, 0, 19U);
    if(pZeroX) {
        snprintf(lChars, 19U, "0x%16X", pInt);
    } else {
        snprintf(lChars, 17U, "%16X", pInt);
    }

    return String(lChars);
}



bool hexStrToUInt8(const String &pStr, uint8_t &pInt) {
    bool lResult = false;
    unsigned int lInt = 0U;
    if(1 == sscanf(pStr.c_str(), "0x%02X", &lInt)) {
        lResult = true;
    } else if(1 == sscanf(pStr.c_str(), "%02X", &lInt)) {
        lResult = true;
    }

    if((0x0000U != (0xFF00U & lInt)) && (0U != lInt)) {
        lResult = false;
    } else {
        pInt = (uint8_t)(0x00FFU & lInt);
    }

    return lResult;
}

bool hexStrToUInt16(const String &pStr, uint16_t &pInt) {
    bool lResult = false;
    unsigned int lInt = 0U;
    if(1 == sscanf(pStr.c_str(), "0x%04X", &lInt)) {
        lResult = true;
    } else if(1 == sscanf(pStr.c_str(), "%04X", &lInt)) {
        lResult = true;
    }

    if(lResult) {
        pInt = (uint16_t)(0xFFFFU & lInt);
    }

    return lResult;
}
