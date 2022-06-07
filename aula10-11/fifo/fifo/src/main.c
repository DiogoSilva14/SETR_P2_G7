/** @file main.c
 * @brief main.c file brief description
 *
 * This is a program made for the 10-11 class of SETR. Given a value from 0 to 3 V and filtering it by removing the outliers, it generates a pwm signal accordingly.
 * In this approach we will be using threads and a fifo.
 *
 * 
 * @date 31 May 2022
 * @bug No known bugs.
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/adc.h>
#include <drivers/pwm.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <string.h>
#include <timing/timing.h>
#include <stdlib.h>
#include <stdio.h>


/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/* Thread scheduling priority */
#define thread_A_prio 1
#define thread_B_prio 1
#define thread_C_prio 1

/* Therad periodicity (in ms)*/
#define SAMP_PERIOD_MS 1000

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

/* Create fifos*/
struct k_fifo fifo_ab;
struct k_fifo fifo_bc;

/* Create fifo data structure and variables */
struct data_item_t {
    void *fifo_reserved;    /* 1st word reserved for use by FIFO */
    uint16_t data[10];          /* Actual data */
};

/* Thread code prototypes */
void thread_A_code(void *argA, void *argB, void *argC);
void thread_B_code(void *argA, void *argB, void *argC);
void thread_C_code(void *argA, void *argB, void *argC);

/*ADC definitions and includes*/
#include <hal/nrf_saadc.h>
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

#define BUFFER_SIZE 1

const struct device *adc_dev = NULL;
static uint16_t adc_sample_buffer[BUFFER_SIZE] = {0};

/** @brief This function takes one sample from the ADC.
 *  
 *  @return error while reading the adc
 */
static int adc_sample(void){
	int ret;
	const struct adc_sequence sequence = {
		.channels = BIT(ADC_CHANNEL_ID),
		.buffer = adc_sample_buffer,
		.buffer_size = sizeof(adc_sample_buffer),
		.resolution = ADC_RESOLUTION,
	};

	if (adc_dev == NULL) {
            printk("adc_sample(): error, must bind to adc first \n\r");
            return -1;
	}

	ret = adc_read(adc_dev, &sequence);
	if (ret) {
            printk("adc_read() failed with code %d\n", ret);
	}	

	return ret;
}

/* ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};

/* Refer to dts file */
#define PWM0_NID DT_NODELABEL(pwm0) 
#define BOARDLED_PIN 13
                /* Pin at which LED is connected. Addressing is direct (i.e., pin number)                  */
                /* Note 1: The PMW channel must be associated with the SAME pin in the DTS file            */
                /*         See the overlay file in this project to see how to change the assignment        */
                /*         *** Note: RUN CMAKE (Project -> Run Cmake) after editing the overlay file***    */
                /* Note 2: the pin can (and should) be obtained automatically from the DTS file.           */
                /*         I'm doing it manually to avoid entering in (cryptic) DT macros and to force     */ 
                /*         you to read the dts file.                                                       */
                /*         This line would do the trick: #define BOARDLED_PIN DT_PROP(PWM0_NID, ch0_pin)   */

/** @brief The main is responsible for binding the ADC as well as creating the threads 
 *  
 */
void main(void){

	/* Error variable for syscalls */
	int err = 0;

	/* Bind to ADC and print an error message in case it fails */
	adc_dev = device_get_binding(DT_LABEL(ADC_NID));
	if (!adc_dev) {
        printk("ADC device_get_binding() failed\n");
    } 
    err = adc_channel_setup(adc_dev, &my_channel_cfg);
    if (err) {
        printk("adc_channel_setup() failed with error code %d\n", err);
    }

	/* It is recommended to calibrate the SAADC at least once before use, and whenever the ambient temperature has changed by more than 10 °C */
    NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;

    /* Create/Init fifos */
    k_fifo_init(&fifo_ab);
    k_fifo_init(&fifo_bc);
    
    /* Create tasks */
    thread_A_tid = k_thread_create(&thread_A_data, thread_A_stack,
        K_THREAD_STACK_SIZEOF(thread_A_stack), thread_A_code,
        NULL, NULL, NULL, thread_A_prio, 0, K_NO_WAIT);

    thread_B_tid = k_thread_create(&thread_B_data, thread_B_stack,
        K_THREAD_STACK_SIZEOF(thread_B_stack), thread_B_code,
        NULL, NULL, NULL, thread_B_prio, 0, K_NO_WAIT);

    thread_B_tid = k_thread_create(&thread_C_data, thread_C_stack,
        K_THREAD_STACK_SIZEOF(thread_C_stack), thread_C_code,
        NULL, NULL, NULL, thread_C_prio, 0, K_NO_WAIT);

    
    return;

} 

/** @brief This thread is responsible for acquiring the values from the adc. 
 *  
 */
void thread_A_code(void *argA , void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;

	/* Declare struct for fifo */
	struct data_item_t data_ab;

	/* Declare variable for iteration and for syscall error */
	uint8_t i = 0;
	int err = 0;

	/* Clear the buffer */
	for(int i=0; i < 10; i++){
		data_ab.data[i] = 0;
	}

    /* Compute next release instant */
    release_time = k_uptime_get() + SAMP_PERIOD_MS;

    /* Thread loop */
    while(1) {
        
		/* Take a sample from ADC and print error message in case it fails */
		err = adc_sample();

		if(err){
			printk("Error reading ADC\n");
		}

		/* Add the sample to the buffer */
		data_ab.data[i] = adc_sample_buffer[0];

		printk("\n\n\n\nADC: %d \n", data_ab.data[i]);

		i++;

		i = i % 10; 
        
        k_fifo_put(&fifo_ab, &data_ab);

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += SAMP_PERIOD_MS;

        }
    }

}

/** @brief The this thread is responsible for calculating the average excluding the outliers (10% off from average)
 */
void thread_B_code(void *argA , void *argB, void *argC){
	/* Struct pointer and declaration for fifo data */
	struct data_item_t *data_ab;
    struct data_item_t data_bc;

	/* Declare variables for the sum of the values, the count of values summed, the inferior limit and the superior limit */
	uint32_t sum = 0, count = 0;
	float inf_lim;
	float sup_lim;

    while(1) {
		/* Get data struct from fifo */
        data_ab = k_fifo_get(&fifo_ab, K_FOREVER);

	   /* Reset the sum */ 
		sum = 0;

		/* Compute the average of the buffer */
		for(int i=0; i < 10; i++){
			sum += data_ab->data[i];
		}

		data_bc.data[0] = sum/10;

		/* Calculate an inferior and superior limit (10% off from average) */
		inf_lim = data_bc.data[0] * 0.9;
		sup_lim = data_bc.data[0] * 1.1;

		/* Reset the sum and count */
		sum = 0;
		count = 0;

		/* Add the values to the sum excluding the outliers, and count them */
		for(int i=0; i < 10; i++){
			if(!(data_ab->data[i] > sup_lim || data_ab->data[i] < inf_lim)){
				sum += data_ab->data[i];
				count++;
			}
		}

		/* Compute the average except if the count is 0, to avoid dividing by 0 */
		if(count != 0){
			data_bc.data[0] = sum/count;
		}else{
			data_bc.data[0] = 0;
		}

		printk("AVG: %d \n", data_bc.data[0]);

		/* Reset the count and sum variables */
		sum = 0;
		count = 0;

        k_fifo_put(&fifo_bc, &data_bc);      
  }
}

/** @brief The this thread is responsible for generating a PWM signal and outputing it to a LED
 */
void thread_C_code(void *argA , void *argB, void *argC){
    const struct device *pwm0_dev;          /* Pointer to PWM device structure */
	int ret=0;								/* Return variable for syscall errors */
	float duty_cycle = 0;					/* Duty_cycle for the PWM */
	struct data_item_t *data_bc;			/* Struct pointer to the fifo data */

	unsigned int pwmPeriod_us = 1000;       /* PWM priod in us */

	/* Bind to PWM */
	pwm0_dev = device_get_binding(DT_LABEL(PWM0_NID));
    if (pwm0_dev == NULL) {
		printk("Error: Failed to bind to PWM0\n r");
		return;
    }
    else  {
        printk("Bind to PWM0 successful\n\r");            
    }

    while(1) {
		/* Take the data from the FIFO */
        data_bc = k_fifo_get(&fifo_bc, K_FOREVER);

		/* Calculate the duty_cycle from the average value */
		duty_cycle = (float)data_bc->data[0]*100/1023;

		printk("DUTY: %d \n", (int)duty_cycle);
	
		/* Apply the PWM signal and indicate the error in case there is any */
		ret = pwm_pin_set_usec(pwm0_dev, BOARDLED_PIN,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*(unsigned int)duty_cycle)/100), PWM_POLARITY_NORMAL);

		if(ret){
			printk("Error %d setting PWM\n", ret);
		}

	}
}