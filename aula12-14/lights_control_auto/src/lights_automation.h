/** @file lights_automation.h
 * @brief Device Drivers implementation
 *
 * This is a library made for the SETR project. It is respnsible for the control of the automatic mode
 * 
 * @date 20 June 2022
 * @bug No known bugs.
 */
 
#ifndef _LIGHTS_AUTO_H    /* Guard against multiple inclusion */
#define _LIGHTS_AUTO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "PI_controller.h"

/* Defines for the direction of PWM change when in manual mode */
#define UP 1
#define DOWN 2
/* Period of sleep between while loops */
#define PRINT_PERIOD 100

/**
 * @brief Initializes the variables for automation and the PI controller
 *
 */
void init_automation();

/**
 * @brief Get the PWM output value for the system. This value is gotten either
 * from the array of predefined values that the user can scroll through or the
 * PI controller.
 *
 * @return PWM duty cycle
 */
float get_value();

/**
 * @brief Print the menu with system statistics and user interaction via UART
 *
 */
void print_menu();

/**
 * @brief Handle the options inputted by the user via UART. This function blocks 
 * while the user is choosing the parameters to change but includes some delays
 * to prevent the other threads from not running.
 *
 */
void handle_option(uint8_t option);

/**
 * @brief Change between the automatic mode and the manual mode.
 * 
 * @param option true to put the system in automatic mode and false to put the
 * system in manual mode
 *
 */
void select_auto(bool option);

/**
 * @brief Change between the predefined PWM output values
 * 
 * @param direction indicates the direction in which the PWM output value
 * should change, either up or down. Use the UP and DOWN defines to change.
 *
 */
void change_manual(uint8_t direction);

#endif
