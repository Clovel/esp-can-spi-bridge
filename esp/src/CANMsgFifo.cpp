/**
 * @brief CAN Bus message FIFO class implementation
 * 
 * @file CANMsgFifo.cpp
 */

/* Includes -------------------------------------------- */
#include "CANMessage.hpp"
#include "CANMsgFifo.hpp"

/* Defines --------------------------------------------- */

/* Global variables ------------------------------------ */

/* Type definitions ------------------------------------ */

/* CAN Message FIFO class implementation --------------- */
CANMsgFifo::CANMsgFifo(const size_t &pSize) : Fifo(pSize)
{
    /* Empty */
}
