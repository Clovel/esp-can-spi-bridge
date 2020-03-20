/**
 * @brief CAN Bus message FIFO class
 * 
 * @file CANMsgFifo.hpp
 */

#ifndef CANMSGFIFO_HPP
#define CANMSGFIFO_HPP

/* Includes -------------------------------------------- */
#include "CANMessage.hpp"

/* Defines --------------------------------------------- */

/* Global variables ------------------------------------ */

/* Type definitions ------------------------------------ */
/**
 * @brief Defines the state of the FIFO
 */
typedef enum _fifoState {
    FIFO_OK = 0U,   /**< FIFO is neither full nor empty */
    FIFO_FULL,      /**< FIFO is full */
    FIFO_EMPTY,     /**< FIFO is empty */
} fifoState_t;

/* CAN Message FIFO class ------------------------------ */
class CANMsgFifo {
    public:
        CANMsgFifo(const size_t &pSize);

        virtual ~CANMsgFifo();

        /* FIFO managament */
        int getMsg(CANMessage &pMsg);
        int putMsg(const CANMessage &pMsg);

        /* Getters */
        size_t size(void) const;
        fifoState_t state(void) const;

    protected:
        CANMessage *mFifo;              /**< Message-per-message FIFO buffer */
        size_t      mSize;              /**< Message-per-message FIFO buffer size */
        size_t      mRIdx;              /**< Message-per-message FIFO buffer read index */
        size_t      mWIdx;              /**< Message-per-message FIFO buffer write index */
        bool        mRIdxWrapAround;    /**< Read index wraparound flag */
        bool        mWIdxWrapAround;    /**< Write index wraparound flag */
    private:
};

#endif /* CANMSGFIFO_HPP */
