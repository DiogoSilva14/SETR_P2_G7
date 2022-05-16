/** @file device_drivers.c
 * @brief
 *
 * Device drivers for vending machine project
 * 
 * @date 03 May 2022
 * @bug No known bugs.
 */

#include "device_drivers.h"
#include <zephyr.h>
#include <sys/printk.h>
#include <devicetree.h>
#include <device.h>
#include <drivers/gpio.h>

#define GPIO0_NID DT_NODELABEL(gpio0)

#define LED1 0xD
#define LED2 0xE
#define LED3 0xF
#define LED4 0x10

#define BUT_Ext1 0x4
#define BUT_Ext2 28
#define BUT_Ext3 29
#define BUT_Ext4 30

#define BUT1 11
#define BUT2 12
#define BUT3 24
#define BUT4 25

static struct gpio_callback button_cb_data1;
static struct gpio_callback button_cb_data2;
static struct gpio_callback button_cb_data3;
static struct gpio_callback button_cb_data4;


static struct gpio_callback button_board_data1;
static struct gpio_callback button_board_data2;
static struct gpio_callback button_board_data3;
static struct gpio_callback button_board_data4;

static uint8_t button_dir = 0;
static uint8_t event = NONE;
static float coin = 0;



void pressed_button1(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    float temp=1.232;
    printf("\n %.1f \n",temp);
    gpio_pin_toggle(dev, LED1);
	event = EVENT_COIN;
    coin = 0.1;
}


void pressed_button2(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	event = EVENT_COIN;
    coin = 0.2;
}

void pressed_button3(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	event = EVENT_COIN;
    coin = 0.5;
}


void pressed_button4(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	event = EVENT_COIN;
    coin = 1;
}


void pressed_board_1(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	event = EVENT_ARROW;
    button_dir = UP;
}

void pressed_board_2(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	event = EVENT_RETURN;
}


void pressed_board_3(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	event = EVENT_ARROW;
    button_dir = DOWN;
}

void pressed_board_4(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    event = EVENT_SELECT;
}

uint8_t init_drivers(){
    
    const struct device *dev;
	int ret;

	dev = device_get_binding(DT_LABEL(GPIO0_NID));  

    ret = gpio_pin_configure(dev, BUT_Ext1, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}

    ret = gpio_pin_configure(dev, LED1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure(dev, BUT_Ext2, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure(dev, BUT_Ext3, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure(dev, BUT_Ext4, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		return -1;
	}

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

    /*Extern Button1*/
    ret = gpio_pin_interrupt_configure(dev, BUT_Ext1, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_cb_data1, pressed_button1, BIT(BUT_Ext1));

	gpio_add_callback(dev, &button_cb_data1);

    
    /*Extern Button2*/
    ret = gpio_pin_interrupt_configure(dev, BUT_Ext2, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_cb_data2, pressed_button2, BIT(BUT_Ext2));

	gpio_add_callback(dev, &button_cb_data2);

    
    ret = gpio_pin_interrupt_configure(dev, BUT_Ext3, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_cb_data3, pressed_button3, BIT(BUT_Ext3));

	gpio_add_callback(dev, &button_cb_data3);

	
	
	ret = gpio_pin_interrupt_configure(dev, BUT_Ext4, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

	/*Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_cb_data4, pressed_button4, BIT(BUT_Ext4));

	gpio_add_callback(dev, &button_cb_data4);




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
    k_msleep(ms);
}

float get_coin_val(){
    float return_val = coin;
    coin = 0;
    return return_val;
}