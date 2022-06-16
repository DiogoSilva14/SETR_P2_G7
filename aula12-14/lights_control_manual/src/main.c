/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <string.h>
#include <timing/timing.h>
#include <stdlib.h>
#include <stdio.h>
#include "nrfx_pwm.h"
#include "init_config.h"
#include <drivers/pwm.h>
#include <hal/nrf_saadc.h>



/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/* Thread scheduling priority */
#define thread_A_prio 1
#define thread_B_prio 1
#define thread_C_prio 1


/* Therad periodicity (in ms)*/
#define thread_A_period 100


/* Create thread stack space */
K_THREAD_STACK_DEFINE(thread_A_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_B_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_C_stack, STACK_SIZE);
  
/* Create variables for thread data */
struct k_thread thread_A_data;
struct k_thread thread_B_data;
struct k_thread thread_C_data;

/* Create task IDs */
k_tid_t thread_A_tid;
k_tid_t thread_B_tid;
k_tid_t thread_C_tid;

/* Global vars (shared memory between tasks A/B and B/C, resp) */
uint32_t buffer[10];
uint32_t avg = 0;

/* Semaphores for task synch */
struct k_sem sem_aa;
struct k_sem sem_ab;
struct k_sem sem_bc;

   /* Create and init semaphores */
    

/* Thread code prototypes */
void thread_A_code(void *argA, void *argB, void *argC);
void thread_B_code(void *argA, void *argB, void *argC);
void thread_C_code(void *argA, void *argB, void *argC);
static uint8_t pwm;	


#define GPIO0_NID DT_NODELABEL(gpio1)
/* PWM settings */
/* Refer to dts file */
#define PWM0_NID DT_NODELABEL(pwm0)
#define pwm_channel 13
#define BOARDLED_PIN 13
                /* Pin at which LED is connected. Addressing is direct (i.e., pin number)                  */
                /* Note 1: The PMW channel must be associated with the SAME pin in the DTS file            */
                /*         See the overlay file in this project to see how to change the assignment        */
                /*         *** Note: RUN CMAKE (Project -> Run Cmake) after editing the overlay file***    */
                /* Note 2: the pin can (and should) be obtained automatically from the DTS file.           */
                /*         I'm doing it manually to avoid entering in (cryptic) DT macros and to force     */ 
                /*         you to read the dts file.                                                       */
                /*         This line would do the trick: #define BOARDLED_PIN DT_PROP(PWM0_NID, ch0_pin)   */


void main(void)
{
	init_drivers();
	init_adc();
	k_sem_init(&sem_aa, 1, 1);
	k_sem_init(&sem_ab, 0, 1);
    k_sem_init(&sem_bc, 0, 1);
	
    
    /* Create tasks */
    /*thread_A_tid = k_thread_create(&thread_A_data, thread_A_stack,
        K_THREAD_STACK_SIZEOF(thread_A_stack), thread_A_code,
        NULL, NULL, NULL, thread_A_prio, 0, K_NO_WAIT);*/

    /*thread_B_tid = k_thread_create(&thread_B_data, thread_B_stack,
        K_THREAD_STACK_SIZEOF(thread_B_stack), thread_B_code,
        NULL, NULL, NULL, thread_B_prio, 0, K_NO_WAIT);*/

    thread_C_tid = k_thread_create(&thread_C_data, thread_C_stack,K_THREAD_STACK_SIZEOF(thread_C_stack), thread_C_code,NULL, NULL, NULL, thread_B_prio, 0, K_NO_WAIT);
	
}

/* Thread code implementation */
void thread_A_code(void *argA , void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;

	uint8_t i = 0, err = 0;

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_A_period;

	for(int y=0; y < 10; y++){
		buffer[y] = 0;
	}

    /* Thread loop */
    while(1) {
        
		err = adc_sample();

		if(err){
			printk("Error reading ADC\n");
		}

		buffer[i] = get_adc_buffer();

		i++;

		i = i % 10; 
        
        k_sem_give(&sem_ab);

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += thread_A_period;

        }
    }
}

void thread_B_code(void *argA , void *argB, void *argC){

	uint32_t sum = 0;
	float inf_lim;
	float sup_lim;

    while(1) {
        k_sem_take(&sem_ab,  K_FOREVER);
        
		sum = 0;

		for(int i=0; i < 10; i++){
			sum += buffer[i];
		}

		avg = sum/10;

		inf_lim = avg * 0.9;
		sup_lim = avg * 1.1;

		for(int i=0; i < 10; i++){
			if(buffer[i] > sup_lim || buffer[i] < inf_lim){
				buffer[i] = avg;
			}
		}

		sum = 0;

		for(int i=0; i < 10; i++){
			sum += buffer[i];
		}

		avg = sum/10;

		sum = 0;

        k_sem_give(&sem_bc);      
  }
}


/** @brief This thread is responsible for setting the PWM values. 
 *  
 */
void thread_C_code(void *argA , void *argB, void *argC){

	const struct device *dev;
	dev = device_get_binding(DT_LABEL(GPIO0_NID));  

	/* Pointer to PWM device structure */
	const struct device *pwm0_dev; 
       
	
	/* Return variable for syscall errors */
	int ret=0;	

	/* Duty_cycle for the PWM */
	uint8_t duty_cycle = 0;												
	
	/* PWM period in us */
	unsigned int pwmPeriod_us = 1000;    

	/* Bind to PWM */
	pwm0_dev = device_get_binding(DT_LABEL(PWM0_NID));	
    if (pwm0_dev == NULL) {
		printk("Error: Failed to bind to PWM0\n r");
		return;
    }
    else  {
        printk("Bind to PWM0 successful\n\r");            
    }


	/* Thread loop */
    while(1) {
		get_pwm_value(&pwm);

		duty_cycle = (float)(100-pwm);

		if (get_state()){
			printk("DUTY: %d \r", (int)(100-duty_cycle));
						
			/* Apply the PWM signal and indicate the error in case there is any */
			ret = pwm_pin_set_usec(pwm0_dev, BOARDLED_PIN,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*(unsigned int)duty_cycle)/100), PWM_POLARITY_NORMAL);

			if(ret){
				printk("Error %d setting PWM\n", ret);
			}

		/* Apply the PWM signal and indicate the error in case there is any */
		
			ret = pwm_pin_set_usec(pwm0_dev, pwm_channel,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*((unsigned int)duty_cycle)/100)), PWM_POLARITY_NORMAL);

			if(ret){
				printk("Error %d setting PWM\n", ret);
			}
		}
		else{
			printk("OFF\r");
		}
    }
}