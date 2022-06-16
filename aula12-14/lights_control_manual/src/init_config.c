#include "init_config.h"
#include <zephyr.h>
#include <sys/printk.h>
#include <devicetree.h>
#include <device.h>
#include <drivers/gpio.h>
#include <stdio.h>
#include <drivers/pwm.h>
#include <hal/nrf_saadc.h>
#include <drivers/adc.h>

#define GPIO0_NID DT_NODELABEL(gpio0)

//Button definition 
#define BUT1 11
#define BUT2 12
#define BUT3 24
#define BUT4 25

static struct gpio_callback button_board_data1;
static struct gpio_callback button_board_data2;
static struct gpio_callback button_board_data3;
static struct gpio_callback button_board_data4;

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

#define BUFFER_SIZE 1

const struct device *adc_dev = NULL;
static uint16_t adc_sample_buffer[BUFFER_SIZE];

/* ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};




static uint8_t pwm_values[6]={0, 20, 40, 60, 80, 100};
static uint8_t index=0;

static bool state = true;
//Call back functions

void pressed_board_1(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	//Manual mode
	state = true;
}

void pressed_board_2(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	//printk("Button 2 pressed\n");
    
    if (index < 5)
        index++;

    //printk("%d = %d\n",index, pwm_values[index]);
}


void pressed_board_3(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	//Automatic mode
	index=0;
	state = false;
	
}

void pressed_board_4(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    //printk("Button 4 pressed\n");
    
    if (index > 0)
        index--;
    
    //printk("%d = %d\n",index, pwm_values[index]);
}

uint8_t init_drivers(){
    const struct device *dev;
	int ret;

	dev = device_get_binding(DT_LABEL(GPIO0_NID));  

    // Button configuration

    ret = gpio_pin_configure(dev, BUT1, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure(dev, BUT2, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure(dev, BUT3, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}


	ret = gpio_pin_configure(dev, BUT4, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}

    // Button interruption configuration

    ret = gpio_pin_interrupt_configure(dev, BUT1, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_board_data1, pressed_board_1, BIT(BUT1));

	gpio_add_callback(dev, &button_board_data1);

	ret = gpio_pin_interrupt_configure(dev, BUT2, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_board_data2, pressed_board_2, BIT(BUT2));

	gpio_add_callback(dev, &button_board_data2);


	ret = gpio_pin_interrupt_configure(dev, BUT3, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_board_data3, pressed_board_3, BIT(BUT3));

	gpio_add_callback(dev, &button_board_data3);


	ret = gpio_pin_interrupt_configure(dev, BUT4, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_board_data4, pressed_board_4, BIT(BUT4));

	gpio_add_callback(dev, &button_board_data4);

    return 0;
}


void get_pwm_value(uint8_t* pwm_pointer){
    *pwm_pointer = pwm_values[index];
}

int get_state(){
    if (state){
		return 1;
	}
	else{
		return 0;
	}

}

/* Takes one sample */
int adc_sample(void){
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

uint16_t get_adc_buffer(){
	return adc_sample_buffer[0];
}

void init_adc(){
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
}