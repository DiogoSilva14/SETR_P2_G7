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
    char string[50];

    sprintf(string, "Dispensing product...\n");

    print(string);

    set_user_money(get_user_money() - get_prod_val());
    device_sleep(5000);
}

void cycle_prod(){
    uint8_t dir = get_arrow_val();

    if(dir == UP){
        set_curr_product((get_curr_product()+1) % NUM_PRODUCTS);
    }else{
        set_curr_product(((get_curr_product() + NUM_PRODUCTS) - 1) % NUM_PRODUCTS);
    }
}

void return_money(){
    char string[50];

    sprintf(string, "Returning %.1f €\n", get_user_money());

    print(string);

    set_user_money(0);
}

void error_msg(){
    char string[50];

    sprintf(string, "The product you want costs %.1f but you only have %.1f\n", get_prod_val(), get_user_money());

    print(string);
}

void display_product_money(){
    char prod_name[50];

    switch(get_curr_product()){
        case BEER:
            strcpy(prod_name, "BEER");
            break;
        case TUNA_SANDWICH:
            strcpy(prod_name, "TUNA_SANDWICH");
            break;
        case COFFEE:
            strcpy(prod_name, "COFFEE");
            break;
    }


    char string[200];

    sprintf(string, "%s: %.1f . You have %.1f     \r", prod_name, get_prod_val(), get_user_money());

    print(string);
}

void coin_inserted(){
    set_user_money(get_user_money() + get_coin_val());
}