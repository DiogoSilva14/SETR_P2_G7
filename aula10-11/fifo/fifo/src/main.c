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
static uint16_t adc_sample_buffer[BUFFER_SIZE];

/* Takes one sample */
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

/* Main function */
void main(void){

	int err = 0;

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

/* Thread code implementation */
void thread_A_code(void *argA , void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;

	struct data_item_t data_ab;

	for(int i=0; i < 10; i++){
		data_ab.data[i] = 0;
	}

	uint8_t err = 0, i = 0;

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_A_period;

    /* Thread loop */
    while(1) {
        
		err = adc_sample();

		if(err){
			printk("Error reading ADC\n");
		}

		data_ab.data[i] = adc_sample_buffer[0];

		i++;

		i = i % 10; 
        
        k_fifo_put(&fifo_ab, &data_ab);

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += thread_A_period;

        }
    }

}

void thread_B_code(void *argA , void *argB, void *argC){
	struct data_item_t *data_ab;
    struct data_item_t data_bc;

	uint32_t sum = 0;
	float inf_lim;
	float sup_lim;

    while(1) {
        data_ab = k_fifo_get(&fifo_ab, K_FOREVER);
        
		sum = 0;

		for(int i=0; i < 10; i++){
			sum += data_ab->data[i];
		}

		data_bc.data[0] = sum/10;

		inf_lim = data_bc.data[0] * 0.9;
		sup_lim = data_bc.data[0] * 1.1;

		for(int i=0; i < 10; i++){
			if(data_ab->data[i] > sup_lim || data_ab->data[i] < inf_lim){
				data_ab->data[i] = data_bc.data[0];
			}
		}

		sum = 0;

		for(int i=0; i < 10; i++){
			sum += data_ab->data[i];
		}

		data_bc.data[0] = sum/10;

		sum = 0;

        k_fifo_put(&fifo_bc, &data_bc);      
  }
}

void thread_C_code(void *argA , void *argB, void *argC){
    const struct device *pwm0_dev;          /* Pointer to PWM device structure */
	int ret=0;
	float duty_cycle = 0;
	struct data_item_t *data_bc;

	unsigned int pwmPeriod_us = 1000;       /* PWM priod in us */

	pwm0_dev = device_get_binding(DT_LABEL(PWM0_NID));
    if (pwm0_dev == NULL) {
		printk("Error: Failed to bind to PWM0\n r");
		return;
    }
    else  {
        printk("Bind to PWM0 successful\n\r");            
    }

    while(1) {
        data_bc = k_fifo_get(&fifo_bc, K_FOREVER);

		duty_cycle = (float)data_bc->data[0]*100/1023;
	
		ret = pwm_pin_set_usec(pwm0_dev, BOARDLED_PIN,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*(unsigned int)duty_cycle)/100), PWM_POLARITY_NORMAL);

		if(ret){
			printk("Error %d setting PWM\n", ret);
		}

	}
}