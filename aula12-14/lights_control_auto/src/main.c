#include <stdlib.h>
#include <stdio.h>
#include "device_drivers.h"
#include "lights_automation.h"

/* Thread code prototypes */
void thread_A_code(void *argA, void *argB, void *argC);
void thread_B_code(void *argA, void *argB, void *argC);
void thread_C_code(void *argA, void *argB, void *argC);

/* PWM Output val */
float pwm_val = 0;

/**
 * @brief Main function
 *
 * Initializes the device drivers, light automation system and the threads. After that the 
 * main function loop is responsible for user interaction with the objective of changing
 * the system behavior and/or tune the system parameters.
 *
 */
void main(void){
	init_drivers();
  init_automation();

	sem_init(SEM_AA, 1, 1);
	sem_init(SEM_AB, 0, 1);
	sem_init(SEM_BC, 0, 1);
    
	thread_create(THREAD_A, thread_A_code);
	thread_create(THREAD_B, thread_B_code);
	thread_create(THREAD_C, thread_C_code);	

  uint32_t print_time;

  while(1){

    print_time = get_uptime();

    print_menu();

    if(uart_rx_rdy()){
      handle_option(get_char());
    }

    if(get_button_press(1)){
      select_auto(true);
    }
    if(get_button_press(2)){
      change_manual(UP);
    }
    if(get_button_press(3)){
      select_auto(false);
    }
    if(get_button_press(4)){
      change_manual(DOWN);
    }

    while(get_uptime() < (print_time + PRINT_PERIOD)){
      sleep_ms(PRINT_PERIOD);
    }

  }

}

/**
 * @brief Thread A code
 * 
 * Thread A is responsible for sampling the ADC value via device drivers
 *
 */
void thread_A_code(void *argA , void *argB, void *argC){
  /* Timing variables to control task periodicity */
  int64_t fin_time=0, release_time=0;

  /* Compute next release instant */
  release_time = get_uptime() + thread_A_period;

  /* Thread loop */
  while(1) {

    adc_sample();

    sem_give(SEM_AB);      

    /* Wait for next release instant */ 
    fin_time = get_uptime();

    if(fin_time < release_time) {
      sleep_ms(release_time - fin_time);
      release_time += thread_A_period;
    }
  }
}

/**
 * @brief Thread B code
 * 
 * Thread B is responsible for getting the PWM output value from the light automation system,
 * which runs the PI controller or the manually changed value.
 *
 */

void thread_B_code(void *argA , void *argB, void *argC){
  /* Thread loop */
  while(1) {
    sem_take(SEM_AB);

    pwm_val = get_value();

    sem_give(SEM_BC);     
  }
}

/**
 * @brief Thread C code
 * 
 * Thread C is responsible for outputting the PWM value.
 *
 */

void thread_C_code(void *argA , void *argB, void *argC){
	/* Thread loop */
  while(1) {
    sem_take(SEM_BC);

    pwm_duty_cycle(pwm_val);
  }
}