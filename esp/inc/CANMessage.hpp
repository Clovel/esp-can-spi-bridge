/**
 * @brief CAN Bus message class
 * 
 * @file CANMessage.hpp
 */

#ifndef CANMESSAGE_HPP
#define CANMESSAGE_HPP

/* Includes -------------------------------------------- */
#include <Arduino.h>

/* Defines --------------------------------------------- */
#define CAN_MSG_MAX_LEN 8U

/* Global variables ------------------------------------ */

/* CAN Message class ----------------------------------- */
class CANMessage {
    public:
        uint32_t id;
        uint8_t  dlc;
        uint8_t  flags;
        uint8_t  data[CAN_MSG_MAX_LEN];

        CANMessage(const uint32_t &pID,
            const uint8_t &pDLC,
            const uint8_t &pFlags,
            const uint8_t *pData = nullptr);

        CANMessage operator=(const CANMessage &pOther);
};


#endif /* CANMESSAGE_HPP */
