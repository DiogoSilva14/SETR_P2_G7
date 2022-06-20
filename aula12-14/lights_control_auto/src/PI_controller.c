#include "PI_controller.h"

long unsigned int last_ms = 0;
float controller_setpoint = 0;


void PI_init(float setpoint){
    controller_setpoint = setpoint;
    last_ms = get_uptime();
}

float PI_loop(float temp){
    static float last_error = 0;
    static float last_integral = 0;
    
    float error = controller_setpoint - temp;
    
    float delta_ms = (float)(get_uptime()- last_ms);
    last_ms = get_uptime();
    
    float new_integral = last_integral + Ki*(error + last_error)*delta_ms/2;
    
    if(new_integral > 100){
        new_integral = last_integral;
    }else if(new_integral <= 0){
        new_integral = last_integral;
    }
    
    float output = Kp*error + new_integral;
    
    last_integral = new_integral;
    last_error = error;
    
    if(output < 0){
        output = 0;
    }else if(output > 100){
        output = 100;
    }
    
    return output;
}

void PI_change_setpoint(float new_setpoint){
    controller_setpoint = new_setpoint;
}

float get_setpoint(){
    return controller_setpoint;
}