/** 
 * @file AddrFilter.cpp
 */

/* Includes -------------------------------------------- */
#include "can-232.h"

#include <stdint.h>

/* Defines --------------------------------------------- */

/* Addres Filter function ------------------------------ */
int8_t myCustomAddressFilter(const int32_t &pAddr) {
    int8_t ret = LW232_FILTER_SKIP; //LW232_FILTER_PROCESS or LW232_FILTER_SKIP
    switch(pAddr) {
        case 0x01b: //VIN
        case 0x1C8:  //lights
        case 0x2C0: // pedals
        case 0x3d0: // sound vol, treb..
            ret = LW232_FILTER_PROCESS;
            break;
        case 0x000: // ?
        case 0x003: //shifter
        case 0x015: // dor open close affects this as well
        case 0x02c: // ???
            ret = LW232_FILTER_SKIP;
            break;
        case 0x002:
        case 0x1a7: //fuel cons or some other
            ret = LW232_FILTER_PROCESS;
            break;
        default: 
            ret = LW232_FILTER_SKIP;
            break;
    }

    return ret;
}