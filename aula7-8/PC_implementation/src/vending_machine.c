/** @file vending_machine.c
 * @brief vending_machine.c file brief description
 *
 * Vending machine state machine implementation file
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

#include "vending_machine.h"

uint8_t* curr_prod;
uint8_t* prod_val;
uint8_t* user_val;

uint8_t get_curr_product(){

}

int main(){

    init_sm();

    while(1){
        sm_loop();
    }

    return 0;
}