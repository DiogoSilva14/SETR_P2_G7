/** @file state_machine.c
 * @brief state_machine.c file brief description
 *
 * State machine state machine implementation file
 * 
 * @date 10 May 2022
 * @bug No known bugs.
 */

#include "state_machine.h"

uint8_t* curr_state;
uint8_t* next_state;
uint8_t* last_event;
uint8_t* curr_prod;
uint8_t* prod_val;
uint8_t* user_val;

void init_sm(){
    curr_state = STATE_DISPLAY;
    next_state = STATE_DISPLAY;
    last_event = NULL;
}

void sm_loop(){
    switch(curr_state){
        case STATE_DISPLAY:
            break;
        case STATE_RETURN_MONEY:
            break;
        case STATE_COIN_INSERTED:
            break;
        case STATE_DISPENSE_PRODUCT:
            break;
        case STATE_ERROR_MESSAGE:
            break;
        case STATE_CHANGE_PRODUCT:
            break;
        default:
            printf("ERROR: Invalid state, possible program error");
            break;
    }
}