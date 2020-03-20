/**
 * @brief CAN Bus message FIFO class
 * 
 * @file CANMsgFifo.hpp
 */

#ifndef CANMSGFIFO_HPP
#define CANMSGFIFO_HPP

/* Includes -------------------------------------------- */
#include "CANMessage.hpp"

#include "Fifo.hpp"

/* Defines --------------------------------------------- */

/* Global variables ------------------------------------ */

/* Type definitions ------------------------------------ */

/* CAN Message FIFO class ------------------------------ */
class CANMsgFifo : public Fifo<CANMessage>{
    public:
        CANMsgFifo(const size_t &pSize);

    protected:
    private:
};

#endif /* CANMSGFIFO_HPP */
