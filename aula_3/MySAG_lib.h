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
 */

typedef struct MySAG {
    uint32_t data[MAX_SIZE];
    uint16_t size;
} MySAG;

/** @brief This function inits the MySAG type. The data array is set all zeroes
 *  @param mysag_array pointer to the MySAG array
 *  @return 0 if succesful
 */

int32_t MySAGInit(MySAG* mysag_array);

/** @brief This function calculates the average value of array
 *  @param mysag_array pointer to the MySAG array
 *  @return average value
 */

uint32_t MySagAvg(MySAG *mysag_array);

/**
*   @brief This function returns the max value inside the data array
*
*   @param mysag pointer to the data structer
**/
uint32_t MySAGMax(MySAG *mysag);
/** @brief This function inserts values to the array
 *  @param mysag_array pointer to the MySAG array
 *         value is an unsigned integer to be added to the array
 *  @return 0 if succesful
 *          -1 if array is full
 */

int32_t MySAGInsert(MySAG* mysag_array, uint32_t value);

/** @brief This function calculates the number of appearences of a certain number in the array
 *  @param mysag_array pointer to the MySAG array
 *  @param value value to be found on array
 *  @return average value
 */

int32_t MySAGFreq(MySAG* mysag_array, uint32_t value)