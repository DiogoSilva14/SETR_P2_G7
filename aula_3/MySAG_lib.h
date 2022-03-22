/** @file MySAG_lib.h
 * @brief MySAG_lib.h file brief description
 *
 * MySAG is a library made for the third class of SETR
 * 
 * @date 22 March 2022
 * @bug No known bugs.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_SIZE 100

/** @brief Struct for MySAG type
 *  The data is an unsigned 32-bit integer array with the size defined by the MAX_SIZE define
 *  The size is an unsigned 8-bit integer because it is enough for numbers until 65535 (our MAX_SIZE is 100) 
 *  The initialized flag is to be verified by every function to prevent them from being used without initializing the MySAG
 */

typedef struct MySAG {
    uint32_t data[MAX_SIZE];
    uint16_t size;
} MySAG;

uint32_t mysag_min(MySAG *mysag);



