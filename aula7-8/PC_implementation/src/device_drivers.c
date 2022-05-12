/** @file device_drivers.c
 * @brief
 *
 * Device drivers for vending machine project
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

#include "device_drivers.h"

static uint8_t button_dir = 0;
static uint8_t event = NONE;
static float coin = 0;

void *button_callback(){
    char c;
    
    while(1){
        c = getchar();

        switch(c){
            case '1': // 1 -> 0.10€ Coin
                event = EVENT_COIN;
                coin = 0.1;
                break;

            case '2': // 2 -> 0.20€ Coin
                event = EVENT_COIN;
                coin = 0.2;
                break;

            case '3': // 3 -> 0.50€ Coin
                event = EVENT_COIN;
                coin = 0.5;
                break;

            case '4': // 4 -> 1€ Coin
                event = EVENT_COIN;
                coin = 1;
                break;

            case 'u': // u -> UP
                event = EVENT_ARROW;
                button_dir = UP;
                break;

            case 'd': // d -> DOWN
                event = EVENT_ARROW;
                button_dir = DOWN;
                break;

            case 'r': // r -> RETURN
                event = EVENT_RETURN;
                break;

            case 's': // s -> SELECT
                event = EVENT_SELECT;
                break;
        }    
    }
}

uint8_t init_drivers(){
    // In the PC implementation since we don't have interrupts we will use
    // threads. One thread will be executing getchar() to simulate button
    // interrupts
    pthread_t button_thread;

    if(pthread_create(&button_thread, NULL, button_callback, NULL) != 0){
        return -1;
    }
    //if(pthread_detach(button_callback) != 0){
    //    return -1;
    //}
    
    return 0;
}

void get_last_event(uint8_t* event_pointer){
    *event_pointer = event;
    event = NONE;
}

uint8_t get_arrow_val(){
    return button_dir;
}

void print(char* string){
    printf("%s", string);
}

void device_sleep(uint32_t ms){
    float s = (float)ms / 1000;
    sleep(s); 
}

float get_coin_val(){
    float return_val = coin;
    coin = 0;
    return return_val;
}