/**
 * @brief General usage FIFO class
 * 
 * @file Fifo.hpp
 */

#ifndef FIFO_HPP
#define FIFO_HPP

/* Includes -------------------------------------------- */

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

/* FIFO class ----------------------------------------- */
template <typename T>
class Fifo {
    public:
        Fifo(const size_t &pSize) : mSize(pSize)
        {
            mFifo = (T *)malloc(mSize * sizeof(T));
        }

        virtual ~Fifo() {
            free(mFifo);
        }

        /* FIFO managament */
        int getMsg(T &pData) {
            /* Copy the data in the output */
            pData = *(mFifo + mRIdx);

            /* Increment the read index of the FIFO */
            mRIdx++;
            if(mSize <= mRIdx) {
                mRIdx = 0U;
                mRIdxWrapAround = !mRIdxWrapAround;
            }

            return 0;
        }

        int putMsg(const T &pData) {
            if(FIFO_FULL == state()) {
                return 1;
            }

            /* Copy the data into the Fifo */
            *(mFifo + mWIdx) = pData;

            /* Increment the write index of the FIFO */
            mWIdx++;
            if(mSize <= mWIdx) {
                mWIdx = 0U;
                mWIdxWrapAround = !mWIdxWrapAround;
            }

            return 0;
        }

        /* Getters */
        size_t size(void) const {
            return mSize;
        }

        fifoState_t state(void) const {
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
        };

    protected:
        CANMessage *mFifo;              /**< FIFO buffer */
        size_t      mSize;              /**< FIFO buffer size */
        size_t      mRIdx;              /**< FIFO buffer read index */
        size_t      mWIdx;              /**< FIFO buffer write index */
        bool        mRIdxWrapAround;    /**< Read index wraparound flag */
        bool        mWIdxWrapAround;    /**< Write index wraparound flag */

    private:
};

#endif /* FIFO_HPP */
