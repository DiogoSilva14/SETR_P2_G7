/** @file state_machine.c
 * @brief state_machine.c file brief description
 *
 * State machine state machine implementation file
 * 
 * @date 10 May 2022
 * @bug No known bugs.
 */

#include "state_machine.h"

static uint8_t curr_state;
static uint8_t next_state;
static uint8_t last_event;

uint8_t init_sm(){
    curr_state = STATE_DISPLAY;
    next_state = STATE_DISPLAY;
    last_event = 0;

    return init_drivers();
}

void sm_loop(){
    switch(curr_state){
        case STATE_DISPLAY:
            display_product_money();

            next_state = STATE_DISPLAY;

            get_last_event(&last_event);

            if(last_event == NONE){
                ;
            }else if(last_event == EVENT_SELECT && (get_user_money() >= get_prod_val())){
                next_state = STATE_DISPENSE_PRODUCT;
            }else if(last_event == EVENT_SELECT && (get_user_money() < get_prod_val())){
                next_state = STATE_ERROR_MESSAGE;
            }else if(last_event == EVENT_ARROW){
                next_state = STATE_CHANGE_PRODUCT;
            }else if(last_event == EVENT_RETURN){
                next_state = STATE_RETURN_MONEY;
            }else if(last_event == EVENT_COIN){
                next_state = STATE_COIN_INSERTED;
            }

            break;
        case STATE_RETURN_MONEY:
            return_money();

            next_state = STATE_DISPLAY;
            break;
        case STATE_COIN_INSERTED:
            coin_inserted();

            next_state = STATE_DISPLAY;
            break;
        case STATE_DISPENSE_PRODUCT:
            dispense_prod();

            next_state = STATE_DISPLAY;
            break;
        case STATE_ERROR_MESSAGE:
            error_msg();

            next_state = STATE_DISPLAY;
            break;
        case STATE_CHANGE_PRODUCT:
            cycle_prod();

            next_state = STATE_DISPLAY;
            break;
        default:
            printf("ERROR: Invalid state, possible program error");
            break;
    }

    curr_state = next_state;
}