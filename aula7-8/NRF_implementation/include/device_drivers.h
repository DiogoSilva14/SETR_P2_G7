/** @file device_drivers.h
 * @brief
 *
 * Device drivers for vending machine project
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

#ifndef DEVICE_DRIVERS_H
#define DEVICE_DRIVERS_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#ifdef __linux__
    #include <pthread.h>
    #include <unistd.h>
    #include <pthread.h>
#else
    #include <zephyr.h>
    #include <sys/printk.h>
    #include <device.h>
#endif

// Enum the possible events
enum EVENTS{NONE, EVENT_COIN, EVENT_RETURN, EVENT_ARROW, EVENT_SELECT};

// Enum the buttons for selection of products
enum DIRECTION{UP, DOWN};

/** @brief Initialize device drivers (GPIOs in the case of uC, etc)
 *  @return 0 if successful
 *          -1 indicates initializing failure
 */

uint8_t init_drivers();

/** @brief Changes the passed pointer value to the last event
 *  @param event_pointer pointer to the local to store the last event
 */

void get_last_event(uint8_t* event_pointer);

/** @brief Returns the value of the last arrow pressed (UP or DOWN)
 *  @return unsigned 8-bit integer with value equal to UP or DOWN
 */

uint8_t get_arrow_val();

/** @brief Prints passed string to the screen
 *  @param string String to be printed
 */

void print(char* string);

/** @brief Makes the device sleep for X miliseconds
 *  @param ms Amount of milliseconds for the device to sleep
 */

void device_sleep(uint32_t ms);

/** @brief Returns the value of the last coin inserted
 *  @return value of the coin inserted as a float
 */

float get_coin_val();

#endif