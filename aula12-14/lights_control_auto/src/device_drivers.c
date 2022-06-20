#include "device_drivers.h"

bool verbose = false;

static struct gpio_callback button_board_data1;
static struct gpio_callback button_board_data2;
static struct gpio_callback button_board_data3;
static struct gpio_callback button_board_data4;

const struct device *adc_dev = NULL;
static uint16_t adc_sample_buffer[ADC_BUFFER_SIZE];

/* ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};

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

/* Semaphores for task synch */
struct k_sem sem_aa;
struct k_sem sem_ab;
struct k_sem sem_bc;

/* Pointer to PWM device structure */
const struct device *pwm0_dev; 

/* Struct for UART configuration (if using default valuies is not needed) */
const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

const struct device *uart_dev;          /* Pointer to device struct */ 
static uint8_t rx_buf[RXBUF_SIZE];      /* RX buffer, to store received data */
static uint8_t rx_chars[RXBUF_SIZE];    /* chars actually received  */
volatile int uart_rx_rdy_flag;          /* Flag to signal main() that a message is available */

/* UART callback function prototype */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);

uint8_t rep_mesg[TXBUF_SIZE];

int err;

bool button_pressed[4];

void pressed_board_1(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    if(verbose){
        printk("Button 1 pressed\n");
	}

	button_pressed[0] = true;
}

void pressed_board_2(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
	if(verbose){
        printk("Button 2 pressed\n");
	}

	button_pressed[1] = true;
}


void pressed_board_3(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
	if(verbose){
        printk("Button 3 pressed\n");
	}
    
	button_pressed[2] = true;	
}

void pressed_board_4(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    if(verbose){
        printk("Button 4 pressed\n");
	}
    
    button_pressed[3] = true;
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

    for(int i=0; i<4; i++){
        button_pressed[i] = false;
    }

    init_adc();

	init_uart();

	init_pwm();

    return 0;
}

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

int init_pwm(){
	/* Bind to PWM */
	pwm0_dev = device_get_binding(DT_LABEL(PWM0_NID));	
    if (pwm0_dev == NULL) {
		printk("Error: Failed to bind to PWM0\n r");
		return -1;
    }else{
		if(verbose){
			printk("Bind to PWM0 successful\n\r"); 
		}

		return 0;       
    }
}

void pwm_duty_cycle(float new_duty_cycle){
	if(verbose){
		printk("DUTY_CYCLE: %d \r", (int)(100 - new_duty_cycle));
	}

	int ret;

	/* Apply the PWM signal and indicate the error in case there is any */
	ret = pwm_pin_set_usec(pwm0_dev, PWM_OUTPUT_PIN,
		PWM_PERIOD_USEC,(unsigned int)((PWM_PERIOD_USEC*(unsigned int)(100 - new_duty_cycle))/100), PWM_POLARITY_NORMAL);

	if(ret){
		printk("Error %d setting PWM\n", ret);
	}
}

bool get_button_press(uint8_t button_num){
	uint8_t val = button_pressed[button_num - 1];

	button_pressed[button_num - 1] = false;

	return val;
}

bool button_rdy(){
	return button_pressed[0] || button_pressed[1] || button_pressed[2] || button_pressed[3];
}

int64_t get_uptime(){
	return k_uptime_get();
}

void sleep_ms(int32_t ms){
	k_msleep(ms);
}

int sem_init(uint8_t semaph, uint8_t start_val, uint8_t max_val){
	if(semaph == SEM_AA){
		return k_sem_init(&sem_aa, start_val, max_val);
	}else if(semaph == SEM_AB){
		return k_sem_init(&sem_ab, start_val, max_val);
	}else if(semaph == SEM_BC){
		return k_sem_init(&sem_bc, start_val, max_val);
	}
	
	return -1;
}

void sem_give(uint8_t semaph){
	if(semaph == SEM_AA){
		k_sem_give(&sem_aa);
	}else if(semaph == SEM_AB){
		k_sem_give(&sem_ab);
	}else if(semaph == SEM_BC){
		k_sem_give(&sem_bc);
	}
}

int sem_take(uint8_t semaph){
	if(semaph == SEM_AA){
		return k_sem_take(&sem_aa,  K_FOREVER);
	}else if(semaph == SEM_AB){
		return k_sem_take(&sem_ab,  K_FOREVER);
	}else if(semaph == SEM_BC){
		return k_sem_take(&sem_bc,  K_FOREVER);
	}

	return -1;
}

void thread_create(uint8_t thread_num, void (*fun)()){
	if(thread_num == THREAD_A){
		k_thread_create(&thread_A_data, thread_A_stack,
			K_THREAD_STACK_SIZEOF(thread_A_stack), fun,
			NULL, NULL, NULL, thread_A_prio, 0, K_NO_WAIT);
	}else if(thread_num == THREAD_B){
		k_thread_create(&thread_B_data, thread_B_stack,
			K_THREAD_STACK_SIZEOF(thread_B_stack), fun,
			NULL, NULL, NULL, thread_B_prio, 0, K_NO_WAIT);
	}else if(thread_num == THREAD_C){
		k_thread_create(&thread_C_data, thread_C_stack,
			K_THREAD_STACK_SIZEOF(thread_C_stack), fun,
			NULL, NULL, NULL, thread_B_prio, 0, K_NO_WAIT);
	}
}

void init_uart(){
	/* Bind to UART */
	uart_dev= device_get_binding(DT_LABEL(UART_NID));
    if (uart_dev == NULL) {
        printk("device_get_binding() error for device %s!\n\r", DT_LABEL(UART_NID));
        return;
    }

	/* Configure UART */
    err = uart_configure(uart_dev, &uart_cfg);
    if (err == -ENOSYS) { /* If invalid configuration */
        printk("uart_configure() error. Invalid configuration\n\r");
        return; 
    }	

	/* Register callback */
    err = uart_callback_set(uart_dev, uart_cb, NULL);
    if (err) {
        printk("uart_callback_set() error. Error code:%d\n\r",err);
        return;
    }

	/* Enable data reception */
    err =  uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
    if (err) {
        printk("uart_rx_enable() error. Error code:%d\n\r",err);
        return;
    }
}

/* UART callback implementation */
/* Note that callback functions are executed in the scope of interrupt handlers. */
/* They run asynchronously after hardware/software interrupts and have a higher priority than all threads */
/* Should be kept as short and simple as possible. Heavier processing should be deferred to a task with suitable priority*/
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data){

	switch (evt->type) {
		
		case UART_TX_DONE:
			//printk("UART_TX_DONE event \n\r");
			break;

		case UART_TX_ABORTED:
			//printk("UART_TX_ABORTED event \n\r");
			break;
			
		case UART_RX_RDY:
			//printk("UART_RX_RDY event \n\r");
					/* Just copy data to a buffer. Usually it is preferable to use e.g. a FIFO to communicate with a task that shall process the messages*/
					memcpy(rx_chars,&(rx_buf[evt->data.rx.offset]),evt->data.rx.len); 
					rx_chars[evt->data.rx.len]=0; /* Terminate the string */
					uart_rx_rdy_flag = 1;
			break;

		case UART_RX_BUF_REQUEST:
			//printk("UART_RX_BUF_REQUEST event \n\r");
			break;

		case UART_RX_BUF_RELEASED:
			//printk("UART_RX_BUF_RELEASED event \n\r");
			break;
			
		case UART_RX_DISABLED: 
					/* When the RX_BUFF becomes full RX is is disabled automaticaly.  */
					/* It must be re-enabled manually for continuous reception */
					//printk("UART_RX_DISABLED event \n\r");
			err =  uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
					if (err) {
						printk("uart_rx_enable() error. Error code:%d\n\r",err);              
					}
			break;

		case UART_RX_STOPPED:
			//printk("UART_RX_STOPPED event \n\r");
			break;
			
		default:
			printk("UART: unknown event \n\r");
			break;
	}
}

bool uart_rx_rdy(){
	if(uart_rx_rdy_flag){
		uart_rx_rdy_flag = 0;
		return true;
	}else{
		return false;
	}
}

char get_char(){
	return rx_buf[0];
}