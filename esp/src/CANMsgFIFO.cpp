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
CANMsgFifo::CANMsgFifo(const size_t &pSize) : mSize(pSize)
{
    mFifo = (CANMessage *)malloc(mSize*sizeof(CANMessage));
}

CANMsgFifo::~CANMsgFifo() {
    free(mFifo);
}

/* FIFO managament */
int CANMsgFifo::getMsg(CANMessage &pMsg) {
    pMsg.id    = (mFifo + mRIdx)->id;
    pMsg.dlc   = (mFifo + mRIdx)->dlc;
    pMsg.flags = (mFifo + mRIdx)->flags;

    for(uint8_t i = 0U; (i < pMsg.dlc) && (i < CAN_MSG_MAX_LEN); i++) {
        pMsg.data[i] = (mFifo + mRIdx)->data[i];
    }

    /* Increment the read index of the FIFO */
    mRIdx++;
    if(mSize <= mRIdx) {
        mRIdx = 0U;
        mRIdxWrapAround = !mRIdxWrapAround;
    }

    return 0;
}

int CANMsgFifo::putMsg(const CANMessage &pMsg) {

    if(FIFO_FULL == state()) {
        return 1;
    }

    /* Copy the CAN Message */
    (mFifo + mWIdx)->id    = pMsg.id;
    (mFifo + mWIdx)->dlc  = pMsg.dlc;
    (mFifo + mWIdx)->flags = pMsg.flags;

    for(uint8_t i = 0U; (i < pMsg.dlc) && (i < CAN_MSG_MAX_LEN); i++) {
        (mFifo + mWIdx)->data[i] = pMsg.data[i];
    }

    /* Increment the write index of the FIFO */
    mWIdx++;
    if(mSize <= mWIdx) {
        mWIdx = 0U;
        mWIdxWrapAround = !mWIdxWrapAround;
    }

    return 0;
}

/* Getters */
size_t CANMsgFifo::size(void) const {
    return mSize;
}

fifoState_t CANMsgFifo::state(void) const {
    /* Check if the FIFO is full */
    if(mRIdx == mWIdx) {
        /* Either empty or full */
        if(mRIdxWrapAround != mWIdxWrapAround) {
            /* The FIFO is full, return error */
            return FIFO_FULL;
        } else {
            /* The fifo is empty */
            return FIFO_EMPTY;
        }
    } else {
        /* The FIFO is not empty nor full */
        return FIFO_OK;
    }
}
