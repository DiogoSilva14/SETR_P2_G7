#ifndef _LIGHTS_AUTO_H    /* Guard against multiple inclusion */
#define _LIGHTS_AUTO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "PI_controller.h"

#define UP 1
#define DOWN 2
#define PRINT_PERIOD 100

void init_automation();
float get_value();
void print_menu();
void handle_option(uint8_t option);
void select_auto(bool option);
void change_manual(uint8_t direction);

#endif