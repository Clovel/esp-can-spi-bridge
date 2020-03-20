/**
 * @brief Main source file. Contains the main routines.
 * 
 * @file main.cpp
 */

/* Includes -------------------------------------------- */
#include "def.h"

#include "CANMessage.hpp"
#include "Fifo.hpp"

#include <mcp_can.h>
#include <SPI.h>

#include <Arduino.h>

#include <string>

/* Defines --------------------------------------------- */
#define HSPI_CS_PIN D8
#define SERIAL_FIFO_SIZE 128U

/* Global variables ------------------------------------ */
MCP_CAN *gCAN = nullptr;
Fifo<uint8_t> *gSerialFifo = nullptr;

/* Static variables ------------------------------------ */

/* On-boot routine ------------------------------------- */
void setup(void) {
    /* Set up Serial port */
    Serial.begin(LOG_BAUDRATE);

    /* Set up the CAN port */
    // gCAN = new MCP_CAN(HSPI_CS_PIN);
    // while(CAN_OK != gCAN->begin(CAN_1000KBPS)) {
    //     Serial.println("[ERROR] CAN Bus initialization failed.");
    //     Serial.println("        Trying again...");

    //     /* Waiting 100ms before another init attempt */
    //     delay(100U);
    // }
    // Serial.println("[BOOT ] CAN Bus initialized !");

    gSerialFifo = new Fifo<uint8_t>(SERIAL_FIFO_SIZE);

    /* End of setup */
    Serial.println("[BOOT ] System booted !");
}

/* Loop routine ---------------------------------------- */
void loop(void) {
    // static uint32_t lPayload = 0x00000000U;
    // static CANMessage lTxMsg(0x987U, 8U, 0x00U);
    // static CANMessage lRxMsg(0xFFFU, 8U, 0x00U);

    std::string lDumpStr = "";

    // /* Resetting the data */
    // memset(lTxMsg.data, 0, 8U);

    // /* Incrementing the payload */
    // ++lPayload;

    // /* Setting the CAN payload */
    // for(uint8_t i = 0U; (i < lTxMsg.dlc) && (i < 8U); i++) {
    //     lTxMsg.data[i] = (lPayload >> i) && 0xFFU;
    // }

    /* Sending the CAN data */
    //gCAN->sendMsgBuf(lTxMsg.id, (uint8_t)lTxMsg.flags, lTxMsg.dlc, lTxMsg.data);

    /* Receiving CAN messages */
    // if(CAN_MSGAVAIL == gCAN->checkReceive()) {
    //     /* Read the message */
    //     gCAN->readMsgBuf(&lRxMsg.dlc, lRxMsg.data);
    //     lRxMsg.id = gCAN->getCanId();

    //     Serial.print(lRxMsg.id, HEX);
    //     Serial.print(" [");
    //     Serial.print(lRxMsg.dlc);
    //     Serial.print("] ");
    //     for(uint8_t i = 0U; (i < lRxMsg.dlc) && (i < 8U); i++) {
    //         Serial.print(lRxMsg.data[i], HEX);
    //         Serial.print(" ");
    //     }
    //     Serial.println();
    // }

    /* Receiving Serial data */
    while(FIFO_FULL != gSerialFifo->state() && (0 < Serial.available())) {
        /* Read serial data byte and send back to serial monitor */
        uint8_t lChar = (uint8_t)Serial.read();

        gSerialFifo->putData(lChar);

        // Serial.print("[DEBUG] We got :");
        // Serial.println((char)lChar);
    }

    while(FIFO_EMPTY != gSerialFifo->state()) {
        uint8_t lChar = 0U;

        /* Dump Fifo contents into a string */
        if(0 == gSerialFifo->getData(lChar)) {
            lDumpStr += lChar;
        }
    }

    if(!lDumpStr.empty()) {
        Serial.print(lDumpStr.c_str());
    }

    /* Waiting 5ms before sending the message again. */
    delay(5U);
}
