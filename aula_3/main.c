/** @file main.c
 * @brief main.c file brief description
 *
 * Main file for the MySAG Project
 * 
 * @date 22 March 2022
 * @bug No known bugs.
 */

#include <stdio.h>
#include <stdint.h>
#include "MySAG_lib.h"

MySAG *MySAG_array;

int main(){

    MySAGInit(&MySAG_array);

    for(int i=0; i < 100; i++){
        MySAGInsert(&MySAG_array, i);
        
        if(MySAG_array.data[i] != i){
            printf("Error on insert\n");
        }

        if(MySAGMax(&MySAG_array) != i){
            printf("Error on Max\n");
        }

        if(MySAGMin(&MySAG_array) != 0){
            printf("Error on Min\n");
        }
    }

    return 0;
}