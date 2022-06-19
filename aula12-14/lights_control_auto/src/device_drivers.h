#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>
#include <drivers/adc.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <string.h>
#include <timing/timing.h>
#include <stdlib.h>
#include <stdio.h>
#include "nrfx_pwm.h"
#include <drivers/pwm.h>
#include <hal/nrf_saadc.h>

#define GPIO0_NID DT_NODELABEL(gpio0)

// Buttons used
#define BUT1 11
#define BUT2 12
#define BUT3 24
#define BUT4 25

/*ADC definitions and includes*/
#define ADC_NID DT_NODELABEL(adc) 
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_4
#define ADC_REFERENCE ADC_REF_VDD_1_4
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 40)
#define ADC_CHANNEL_ID 1  

/* This is the actual nRF ANx input to use. Note that a channel can be assigned to any ANx. In fact a channel can */
/*    be assigned to two ANx, when differential reading is set (one ANx for the positive signal and the other one for the negative signal) */  
/* Note also that the configuration of differnt channels is completely independent (gain, resolution, ref voltage, ...) */
#define ADC_CHANNEL_INPUT NRF_SAADC_INPUT_AIN1 
#define ADC_BUFFER_SIZE 1

#define PWM0_NID DT_NODELABEL(pwm0)
#define PWM_OUTPUT_PIN 13
#define PWM_PERIOD_USEC 1000

/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/* Thread scheduling priority */
#define thread_A_prio 1
#define thread_B_prio 1
#define thread_C_prio 1

/* Therad periodicity (in ms)*/
#define thread_A_period 100

#define SEM_AA 1
#define SEM_AB 2
#define SEM_BC 3

#define THREAD_A 1
#define THREAD_B 2
#define THREAD_C 3

void pressed_board_1(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void pressed_board_2(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void pressed_board_3(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void pressed_board_4(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
uint8_t init_drivers();
int adc_sample(void);
uint16_t get_adc_buffer();
void init_adc();
int init_pwm();
void pwm_duty_cycle(float new_duty_cycle);
bool get_button_press(uint8_t button_num);
int64_t get_uptime();
void sleep_ms(int32_t);
int sem_init(uint8_t semaph, uint8_t start_val, uint8_t max_val);
void sem_give(uint8_t semaph);
int sem_take(uint8_t semaph);
void thread_create(uint8_t thread_num, void (*fun)());