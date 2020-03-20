/**
 * @brief Main source file. Contains the main routines.
 * 
 * @file main.cpp
 */

/* Includes -------------------------------------------- */
#include "def.h"

#include "CANMessage.hpp"

#include <mcp_can.h>
#include <SPI.h>

#include <Arduino.h>

/* Defines --------------------------------------------- */
#define HSPI_CS_PIN D8

/* Global variables ------------------------------------ */
MCP_CAN *gCAN = nullptr;

/* Static variables ------------------------------------ */

/* On-boot routine ------------------------------------- */
void setup(void) {
    /* Set up Serial port */
    Serial.begin(LOG_BAUDRATE);

    /* Set up the CAN port */
    gCAN = new MCP_CAN(HSPI_CS_PIN);
    while(CAN_OK != gCAN->begin(CAN_1000KBPS)) {
        Serial.println("[ERROR] CAN Bus initialization failed.");
        Serial.println("        Trying again...");

        /* Waiting 100ms before another init attempt */
        delay(100U);
    }
    Serial.println("[BOOT ] CAN Bus initialized !");

    /* End of setup */
    Serial.println("[BOOT ] System booted !");
}

/* Loop routine ---------------------------------------- */
void loop(void) {
    static uint32_t lPayload = 0x00000000U;
    static CANMessage lTxMsg(0x987U, 8U, 0x00U);
    static CANMessage lRxMsg(0xFFFU, 8U, 0x00U);

    /* Resetting the data */
    memset(lTxMsg.data, 0, 8U);

    /* Incrementing the payload */
    ++lPayload;

    /* Setting the CAN payload */
    for(uint8_t i = 0U; (i < lTxMsg.dlc) && (i < 8U); i++) {
        lTxMsg.data[i] = (lPayload >> i) && 0xFFU;
    }

    /* Sending the CAN data */
    gCAN->sendMsgBuf(lTxMsg.id, (uint8_t)lTxMsg.flags, lTxMsg.dlc, lTxMsg.data);

    /* Receiving CAN messages */
    if(CAN_MSGAVAIL == gCAN->checkReceive()) {
        /* Read the message */
        gCAN->readMsgBuf(&lRxMsg.dlc, lRxMsg.data);
        lRxMsg.id = gCAN->getCanId();

        Serial.print(lRxMsg.id, HEX);
        Serial.print(" [");
        Serial.print(lRxMsg.dlc);
        Serial.print("] ");
        for(uint8_t i = 0U; (i < lRxMsg.dlc) && (i < 8U); i++) {
            Serial.print(lRxMsg.data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    /* Waiting 1s before sending the message again. */
    delay(1000U);
}
