/**
 * @file vAdd.h
 * @author Gonçalo Gomes
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdint.h>

/**
*   @brief This function sums all the values inside a vector
*
*   @param *vector pointer to the vector to the filled
*   @param size size of the vector to be filled. Incorrect size could lead to bad
*   behavior
**/

uint32_t vAdd(uint32_t* num_V, uint32_t size);