#include<stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_SIZE 6

uint8_t init_drivers();

void get_pwm_value(uint8_t* pwm_pointer);

int get_state();

uint16_t get_adc_buffer();

int adc_sample(void);

void init_adc();

