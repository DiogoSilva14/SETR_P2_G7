/** @file vending_machine.h
 * @brief vending_machine.h header file brief description
 *
 * Vending machine state machine implementation header file
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

#include <stdio.h>
#include "state_machine.h"

static enum products{BEER, TUNA_SANDWICH, COFFEE};

#define BEER_PRICE 1.5
#define TUNA_SANDWICH_PRICE 1.0
#define COFFEE_PRICE 0.5

uint8_t get_curr_product();

float get_prod_val();

float get_user_money();

void dispense_prod();

void cycle_prod();

void return_money();

void error_msg();

void display_product_money();

void coin_inserted();