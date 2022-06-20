#ifndef _PI_H    /* Guard against multiple inclusion */
#define _PI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "device_drivers.h"

#define Kp 1
#define Ki 1

/** @brief Initiates the PI with the desired setpoint
 *
 *  @param setpoint Setpoint desired for the PI controller
 */
void PI_init(float setpoint);

/** @brief Calcules the output of the controller for the given input value
 *
 *  @param input_value system output measure for controller
 * 
 *  @return controller output
 */
float PI_loop(float input_value);

void PI_change_setpoint(float new_setpoint);

float get_setpoint();

#endif