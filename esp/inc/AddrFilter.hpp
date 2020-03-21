/** 
 * @file AddrFilter.hpp
 */

#ifndef ADDRFILTER_HPP
#define ADDRFILTER_HPP

/* Includes -------------------------------------------- */
#include <stdint.h>

/* Defines --------------------------------------------- */

/* Addres Filter function ------------------------------ */
/**
 * @brief Optional custom packet filter to reduce number 
 * of messages comingh through to canhacker.
 * 
 * @details Pass this function 
 * as an argument to Can232::setFilter().
 */
int8_t myCustomAddressFilter(const int32_t &pAddr) __attribute__((unused));

#endif /* ADDRFILTER_HPP */
