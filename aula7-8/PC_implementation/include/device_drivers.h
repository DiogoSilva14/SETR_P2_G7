/** @file device_drivers.h
 * @brief
 *
 * Device drivers for vending machine project
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

static enum EVENTS{EVENT_COIN, EVENT_RETURN, EVENT_ARROW, EVENT_SELECT};

void get_last_event(uint8_t* event_pointer);

uint8_t get_arrow_val();