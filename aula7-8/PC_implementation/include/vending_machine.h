/** @file vending_machine.h
 * @brief vending_machine.h header file brief description
 *
 * Vending machine state machine implementation header file
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

#ifndef VENDING_MACHINE_H
#define VENDING_MACHINE_H

#include <stdio.h>
#include <stdint.h>
#include "device_drivers.h"

// Enum the products available in the vending machine
// Always leave the NUM_PRODUCTS as the last value, so that you can easily
// know the number of the products in the enum
enum products{BEER, TUNA_SANDWICH, COFFEE, NUM_PRODUCTS};

// Price of products, same order as the enum
static float prices[] = {1.5, 1.0, 0.5};

/** @brief Returns the currently selected product index
 *  @return Index of the product as an unsigned 8bit integer
 */

uint8_t get_curr_product();

/** @brief Set the new current product index
 *  @param new_prod New selected product index
 */

void set_curr_product(uint8_t new_prod);

/** @brief Returns the value of the currently selected value
 *  @return Value of the product as a float
 */

float get_prod_val();

/** @brief Returns the amount of money the user has available
 *  @return User's money as a float
 */

float get_user_money();

/** @brief Set a new value for the user's money
 *  @param new_val New value for the user's money
 */
void set_user_money(float new_val);

/** @brief Dispenses the selected product
 */

void dispense_prod();

/** @brief Cycles the displayed product up or down
 */

void cycle_prod();

/** @brief Returns the money the user has inserted
 */

void return_money();

/** @brief Displays error message when the user doesn't have enough money
 */

void error_msg();

/** @brief Displays the selected product, its cost and the user money
 */

void display_product_money();

/** @brief Handle a coin being inserted
 */

void coin_inserted();

#endif