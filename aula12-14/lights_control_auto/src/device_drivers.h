/** @file device_drivers.h
 * @brief Device Drivers implementation
 *
 * This is a library made for the SETR project. It is respnsible for the innitialization of the drivers
 * 
 * @date 20 June 2022
 * @bug No known bugs.
 */
 
#ifndef _DEVICE_DRIVERS_H    /* Guard against multiple inclusion */
#define _DEVICE_DRIVERS_H

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
#include <drivers/uart.h>

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

#define UART_NID DT_NODELABEL(uart0)    /* UART Node label, see dts */
#define RXBUF_SIZE 1                   /* RX buffer size */
#define TXBUF_SIZE 60                   /* TX buffer size */
#define RX_TIMEOUT 1000                  /* Inactivity period after the instant when last char was received that triggers an rx event (in us) */

#define SEM_AA 1
#define SEM_AB 2
#define SEM_BC 3

#define THREAD_A 1
#define THREAD_B 2
#define THREAD_C 3

/** 
 * @brief Callback function for button 1 press
 */
void pressed_board_1(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/** 
 * @brief Callback function for button 2 press
 */
void pressed_board_2(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/** 
 * @brief Callback function for button 3 press
 */
void pressed_board_3(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/** 
 * @brief Callback function for button 4 press
 */
void pressed_board_4(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/** 
 * @brief Indicates if any button has been pressed
 * 
 * @return bool indicating if any button has been pressed
 */
bool button_rdy();

/** 
 * @brief Function responsible for initializing everything associated with the device drivers
 * like data structures and binds
 * 
 * @return error number in case of any problem initializing. 0 if successful
 */
uint8_t init_drivers();

/** 
 * @brief Samples a value from the ADC
 * 
 * @return error code in case of error sampling from ADC. 0 if successful
 */
int adc_sample(void);

/** 
 * @brief Gets the value samples from the ADC
 * 
 * @return value sampled by the ADC
 */
uint16_t get_adc_buffer();

/** 
 * @brief Initializes the ADC
 */
void init_adc();

/** 
 * @brief Initializes the PWM module
 * 
 * @return error code in case theres any, 0 if successful
 */
int init_pwm();

/** 
 * @brief Change the PWM output duty cycle
 * 
 * @param new_duty_cycle New duty cycle to be outputed by the PWM module
 */
void pwm_duty_cycle(float new_duty_cycle);

/** 
 * @brief Indicates if the specified button has been pressed
 * 
 * @param button_num button to get press
 * 
 * @return boolean indicated if the speficied button has been pressed or not
 */
bool get_button_press(uint8_t button_num);

/** 
 * @brief Get the microcontroller uptime in miliseconds
 * 
 * @return uptime in miliseconds
 */
int64_t get_uptime();

/** 
 * @brief Sleeps for the amount specified
 * 
 * @param ms miliseconds to sleep
 */
void sleep_ms(int32_t ms);

/** 
 * @brief Initialize the specified semaphores with the specified values
 * 
 * @param semaph Semaphore to be initialized. Use SEM_AA, SEM_AB and SEM_BC defines.
 * @param start_val Starting value of the semaphore
 * @param max_val Maximum value of the semaphore
 */
int sem_init(uint8_t semaph, uint8_t start_val, uint8_t max_val);

/** 
 * @brief Increments the specified semaphore
 * 
 * @param semaph Semaphore to be incremented. Use SEM_AA, SEM_AB and SEM_BC defines.
 */
void sem_give(uint8_t semaph);

/** 
 * @brief Take the specified semaphore
 * 
 * @param semaph Semaphore to be taken. Use SEM_AA, SEM_AB and SEM_BC defines.
 */
int sem_take(uint8_t semaph);

/** 
 * @brief Creates the specified semaphore
 * 
 * @param thread_num Thread to be created. Use THREAD_A, THREAD_B and THREAD_C defines.
 * @param fun function for the thread to run
 */
void thread_create(uint8_t thread_num, void (*fun)());

/** 
 * @brief Initialize UART module
 */
void init_uart();

/** 
 * @brief Indicates if the UART has characters ready to be received
 * 
 * @return bool indicating if there are characters to be read
 */
bool uart_rx_rdy();

/** 
 * @brief Gets the last character read by the UART
 * 
 * @return Last character read by the uart
 */
char get_char();

/** 
 * @brief UART RX callback
 */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);

#endif
