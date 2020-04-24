/**
 * @brief Main source file. Contains the main routines.
 * 
 * @file main.cpp
 */

/* Includes -------------------------------------------- */
#include <Arduino.h>

/* Defines --------------------------------------------- */
#define LOG_BAUDRATE 115200U

/* Global variables ------------------------------------ */

/* Static variables ------------------------------------ */

/* On-boot routine ------------------------------------- */
void setup(void) {
    /* Set up Serial port */
    Serial.begin(LOG_BAUDRATE);

    /* End of setup */
    Serial.println("[BOOT ] Boot successful !");
}

/* Event callbacks ------------------------------------- */

/* Loop routine ---------------------------------------- */
void loop() {
}
