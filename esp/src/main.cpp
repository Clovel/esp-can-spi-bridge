/**
 * @brief Main source file. Contains the main routines.
 * 
 * @file main.cpp
 */

/* Includes -------------------------------------------- */
#include "def.h"
#include "can-232.h"

#include <mcp_can.h>
#include <SPI.h>
#include <SoftwareSerial.h>

/* Defines --------------------------------------------- */
#define HSPI_CS_PIN D8

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* On-boot routine ------------------------------------- */
void setup(void) {
    /* Set up Serial port */
    Serial.begin(LOG_BAUDRATE);

    /* Set up the CAN port */
    Can232::init(CAN_125KBPS, HSPI_CS_PIN);

    /* End of setup */
}

/* Event callbacks ------------------------------------- */
void serialEvent() {
    Can232::serialEvent();
}

/* Loop routine ---------------------------------------- */
void loop() {
    Can232::loop();
}
