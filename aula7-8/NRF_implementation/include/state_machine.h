/** @file state_machine.h
 * @brief state_machine.h header file brief description
 *
 * State machine state machine implementation header file
 * 
 * @date 10 May 2022
 * @bug No known bugs.
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>
#include <stdint.h>
#include "vending_machine.h"
#include "device_drivers.h"

// Enum the states of the state machine
enum STATES{STATE_DISPLAY, STATE_RETURN_MONEY, STATE_COIN_INSERTED, STATE_DISPENSE_PRODUCT, STATE_ERROR_MESSAGE, STATE_CHANGE_PRODUCT};

/** @brief Initialize the state machine
 *  @return 0 if successful
 *          -1 indicates initializing failure
 */
uint8_t init_sm();

/** @brief State machine loop function
 */

void sm_loop();

#endif