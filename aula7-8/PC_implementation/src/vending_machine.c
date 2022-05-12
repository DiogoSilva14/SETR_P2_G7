/** @file vending_machine.c
 * @brief vending_machine.c file brief description
 *
 * Vending machine state machine implementation file
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

#include "vending_machine.h"

// Current selected product index
static uint8_t curr_prod;

// User's money
static float user_val;

uint8_t get_curr_product(){
    return curr_prod;
}

void set_curr_product(uint8_t new_prod){
    curr_prod = new_prod;
}

float get_prod_val(){
    return prices[curr_prod];
}

float get_user_money(){
    return user_val;
}

void set_user_money(float new_val){
    user_val = new_val;
}

void dispense_prod(){
    // Print "dispensing product...."
    set_user_money(get_user_money() - get_prod_val());
    // Sleep for x seconds
}

void cycle_prod(uint8_t dir){
    if(dir == UP){
        new_prod(++get_curr_product() % NUM_PRODUCTS);
    }else{
        new_prod(((get_curr_product() + NUM_PRODUCTS) - 1) % NUM_PRODUCTS);
    }
}

void return_money(){
    // Print returning x money
    set_user_money(0);
}

void error_msg(){
    // Print you dont have money for x product. X money left
}

void display_product_money(){
    // Print you have X
    // Current product costs X
}

void coin_inserted(){
    // Get_Coin_val from drivers
    // Add coin val to user money
}