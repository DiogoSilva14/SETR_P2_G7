#include "lights_automation.h"

uint8_t auto_values[24];
uint8_t current_hour;
bool auto_state = false;
float manual_pwm_vals[] = {0, 25, 50, 75, 100};
int8_t pwm_head = 0;
float controller_out = 0;

void init_automation(){
    for(int i=0; i < 24; i++){
        auto_values[i] = i*4; // test vals
    }

    current_hour = 0;

    PI_init(auto_values[current_hour]);
}

float get_value(){
    if(auto_state){
        controller_out = PI_loop(((float)get_adc_buffer()-200)/8);
        return controller_out;
    }else{
        return manual_pwm_vals[pwm_head]; 
    }
}

void print_menu(){
    printf("\033[2J\033[;H. \033[2J");
    printf("\033[;H");
    printf("====================\n");
    printf(" MODE: ");
    if(auto_state){
        printf("AUTO\n");
    }else{
        printf("MANUAL\n");
    }
    printf(" PWM: %f \n", get_value());
    printf(" ADC: %d\n", get_adc_buffer());
    printf(" DIODE VAL: %f\n", ((float)get_adc_buffer()-200)/8);
    printf(" SETPOINT: %f\n", get_setpoint());
    printf(" CONTROLLER OUT: %f\n", controller_out);
    printf(" Current Hour: %d \n", current_hour);
    printf(" 1 - Show table\n");
    printf(" 2 - Change value\n");
    printf(" 3 - Change hour\n");
    printf(" Select option: ");
}

void handle_option(uint8_t option){
    uint8_t new_hour = 0;

    switch(option){
        case '1':
            printf("====================\n");
            printf("HOUR | VALUE\n");

            for(int i=0; i < 24; i++){
                printf(" %d : %d \n", i, auto_values[i]);
            }

            printf("Press ENTER to continue \n");

            while(!(uart_rx_rdy() && get_char() == '\r')){
                sleep_ms(PRINT_PERIOD);
            }

            break;
        case '2':
            printf("Hour to change: \n");
            
            uint8_t hour_to_change = 0;

            char pressed = 'a';

            while(pressed != '\r'){
                printf("\r%d  ", hour_to_change);

                if(uart_rx_rdy()){
                    pressed = get_char();
                    if(pressed == 'u'){
                        hour_to_change++;
                    }else if(pressed == 'd'){
                        hour_to_change--;
                    }
                }

                if(hour_to_change < 0){
                    hour_to_change = 23;
                }else if(hour_to_change > 23){
                    hour_to_change = 0;
                }

                sleep_ms(PRINT_PERIOD);
            }

            printf("\nCurrent value: %d. New value ? \n", auto_values[hour_to_change]);

            int new_val = 0;

            pressed = 'a';

            while(pressed != '\r'){
                printf("\r%d  ", new_val);

                if(uart_rx_rdy()){
                    pressed = get_char();

                    if(pressed == 'u'){
                        new_val = new_val + 1;
                    }else if(pressed == 'd'){
                        new_val = new_val - 1;
                    }
                }

                if(new_val < 0){
                    new_val = 0;
                }else if(hour_to_change > 100){
                    new_val = 100;
                }

                sleep_ms(PRINT_PERIOD);
            }

            auto_values[hour_to_change] = new_val;

            printf("Value changed successfully!\n");
            printf("Press ENTER to continue \n");

            while(!(uart_rx_rdy() && get_char() == '\r')){
                sleep_ms(PRINT_PERIOD);
            }

            break;
        case '3':
            printf("Current hour: %d. New hour? \n", current_hour);

            pressed = 'a';

            while(pressed != '\r'){
                printf("\r%d  ", new_hour);

                if(uart_rx_rdy()){
                    pressed = get_char();
                    if(pressed == 'u'){
                        new_hour++;
                    }else if(pressed == 'd'){
                        new_hour--;
                    }
                }

                if(new_hour < 0){
                    new_hour = 23;
                }else if(new_hour > 23){
                    new_hour = 0;
                }

                sleep_ms(PRINT_PERIOD);
            }

            current_hour = new_hour;

            PI_change_setpoint(auto_values[current_hour]);

            printf("Value changed successfully!\n");
            printf("Press ENTER to continue \n");

            while(!(uart_rx_rdy() && get_char() == '\r')){
                sleep_ms(PRINT_PERIOD);
            }

            break;

        default:
            printf("ERROR: Option not available\n");
            break;
    }

    PI_change_setpoint(auto_values[current_hour]);
}

void select_auto(bool option){
    auto_state = option;
    PI_change_setpoint(auto_values[current_hour]);
}

void change_manual(uint8_t direction){
    if(direction == UP){
        pwm_head++;
    }else{
        pwm_head--;
    }

    if(pwm_head > 4){
        pwm_head = 4;
    }else if(pwm_head < 0){
        pwm_head = 0;
    }
}