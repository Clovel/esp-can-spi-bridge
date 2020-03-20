/**
 * @brief CAN Bus message class implementation
 * 
 * @file CANMessage.cpp
 */

/* Includes -------------------------------------------- */
#include "CANMessage.hpp"

/* Defines --------------------------------------------- */
#define CAN_MSG_MAX_LEN 8U

/* Global variables ------------------------------------ */

/* CAN Message class implementation -------------------- */
CANMessage::CANMessage(const uint32_t &pID,
    const uint8_t &pDLC,
    const uint8_t &pFlags,
    const uint8_t *pData) : 
    id(pID),
    dlc(pDLC),
    flags(pFlags)
{
    if(nullptr != pData) {
        for(uint8_t i = 0U; (i < dlc) && (i < CAN_MSG_MAX_LEN); i++) {
            data[i] = pData[i];
        }
    }
}
