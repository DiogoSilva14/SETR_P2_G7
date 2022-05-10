/** @file state_machine.h
 * @brief state_machine.h header file brief description
 *
 * State machine state machine implementation header file
 * 
 * @date 10 May 2022
 * @bug No known bugs.
 */

#include <stdio.h>
#include "device_drivers.h"

static enum STATES{STATE_DISPLAY, STATE_RETURN_MONEY, STATE_COIN_INSERTED, STATE_DISPENSE_PRODUCT, STATE_ERROR_MESSAGE, STATE_CHANGE_PRODUCT};
static enum EVENTS{EVENT_COIN, EVENT_RETURN, EVENT_ARROW, EVENT_SELECT};

void init_sm();

void sm_loop();