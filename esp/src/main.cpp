/**
 * @brief Main source file. Contains the main routines.
 * 
 * @file main.cpp
 */

/* Includes -------------------------------------------- */
/* MCP CAN module */
#include "mcp_can.h"

/* Arduino framework */
#include <Arduino.h>

/* Hex tools */
#include "HexTools.hpp"

/* C++ system */
#include <cstring>

/* Defines --------------------------------------------- */
#define LOG_BAUDRATE 115200U

/* GPIO pins */
#define GPIO_4  T0
#define GPIO_0  T1
#define GPIO_2  T2
#define GPIO_15 T3
#define GPIO_13 T4
#define GPIO_12 T5
#define GPIO_14 T6
#define GPIO_27 T7
#define GPIO_33 T8
#define GPIO_32 T9

/* CAN SPI defines */
#define CAN_BAUDRATE            CAN_1000KBPS
#define CAN_MESSAGE_MAX_SIZE    8U
#define CAN_SPI_CS              5   /* VSPI used by default by SPI.h/.cpp for ESP32 Devkit v1 */
#define CAN_SPI_MOSI            19  /* VSPI used by default by SPI.h/.cpp for ESP32 Devkit v1 */
#define CAN_SPI_MISO            23  /* VSPI used by default by SPI.h/.cpp for ESP32 Devkit v1 */
#define CAN_SPI_CLK             18  /* VSPI used by default by SPI.h/.cpp for ESP32 Devkit v1 */

/* Global variables ------------------------------------ */
MCP_CAN gCAN(CAN_SPI_CS);

/* Static variables ------------------------------------ */

/* Helper functions ------------------------------------ */
void printCANMsg(const uint32_t &pCOBID, const uint8_t &pLen, const uint8_t * const pData, const uint8_t &pFlags, const bool &pNewLine = true) {
    Serial.print(uint32ToHexStr(pCOBID));
    Serial.print(" [");
    Serial.print(pLen);
    Serial.print("] ");
    for(uint8_t i = 0U; (i < pLen) && (i < CAN_MESSAGE_MAX_SIZE); ++i) {
        Serial.print(uint8ToHexStr(pData[i], false));
        Serial.print(" ");
    }
    Serial.print("(");
    Serial.print(uint8ToHexStr(pFlags));
    Serial.print(")");
    if(pNewLine) Serial.println();
}

void printTxCANMsg(const uint32_t &pCOBID, const uint8_t &pLen, const uint8_t * const pData, const uint8_t &pFlags, const bool &pNewLine = true) {
    Serial.print("Tx < ");
    printCANMsg(pCOBID, pLen, pData, pFlags, pNewLine);
}

void printRxCANMsg(const uint32_t &pCOBID, const uint8_t &pLen, const uint8_t * const pData, const uint8_t &pFlags, const bool &pNewLine = true) {
    Serial.print("Rx > ");
    printCANMsg(pCOBID, pLen, pData, pFlags, pNewLine);
}

/* On-boot routine ------------------------------------- */
void setup(void) {
    /* Set up Serial port */
    Serial.begin(LOG_BAUDRATE);

    /* Setup CAN bus */
    Serial.println("[BOOT ] Setting up the MCP2515 CAN module");
    if(CAN_OK != gCAN.begin(MCP_ANY, CAN_BAUDRATE, MCP_8MHZ)) {
        Serial.println("[ERROR] CAN setup failed");
        while(true);
    }
    gCAN.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.

    /* End of setup */
    Serial.println("[BOOT ] Boot successful !");
}

/* Event callbacks ------------------------------------- */

/* Loop routine ---------------------------------------- */
void loop(void) {
    static uint32_t sCOBID = 0x001U;
    static uint8_t  sData[CAN_MESSAGE_MAX_SIZE];

    /* Reset data array */
    std::memset(sData, 0U, CAN_MESSAGE_MAX_SIZE);

    /* Build test CAN message */
    sData[0U] = 0x01U;
    sData[1U] = 0x23U;
    sData[2U] = 0x45U;
    sData[3U] = 0x67U;
    sData[4U] = 0x89U;
    sData[5U] = 0xABU;
    sData[6U] = 0xCDU;
    sData[7U] = 0xEFU;

    /* Send the message */
    byte lStatusCode = gCAN.sendMsgBuf(sCOBID, CAN_MESSAGE_MAX_SIZE, sData);
    switch(lStatusCode) {
        case CAN_OK:
            printTxCANMsg(sCOBID, CAN_MESSAGE_MAX_SIZE, sData, 0x00U);
            break;
        case CAN_SENDMSGTIMEOUT:
            Serial.print("[ERROR] ");
            printTxCANMsg(sCOBID, CAN_MESSAGE_MAX_SIZE, sData, 0x00U, false);
            Serial.println(" (Send msg timeout)");
            break;
        case CAN_GETTXBFTIMEOUT:
            Serial.print("[ERROR] ");
            printTxCANMsg(sCOBID, CAN_MESSAGE_MAX_SIZE, sData, 0x00U, false);
            Serial.println(" (Get tx buff time out)");
            break;
        default:
            Serial.print("[ERROR] ");
            printTxCANMsg(sCOBID, CAN_MESSAGE_MAX_SIZE, sData, 0x00U, false);
            Serial.println(" (Unknown)");
            break;
    }

    /* Check COB-ID counter */
    sCOBID = 0x999U < sCOBID ? 0x001U : sCOBID + 1U;

    /* Wait for a second */
    delay(1000U);
}
