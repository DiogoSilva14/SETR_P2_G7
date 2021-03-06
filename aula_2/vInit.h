/** @file vInit.h
 * @brief vInit.h file brief decription 
 *
 * Header file for the implementation of the vInit function
 * 
 * @author Diogo Silva
 * @date 15 March 2022
 * @bug No known bugs.
 */

#ifndef _VINIT_H
#define _VINIT_H

#include <stdio.h>
#include <stdint.h>

/**
*   @brief This function initializes a vector by filling it with natural numbers
*   from 1 to size+1
*
*   @param *vector pointer to the vector to the filled
*   @param size size of the vector to be filled. Incorrect size could lead to bad
*   behavior
**/

void vInit(uint32_t* vector, uint32_t size);

#endif