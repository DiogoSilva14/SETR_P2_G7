#include "MySAG_lib.h"
#include <stdbool.h>

uint32_t mysag_avg(MySAG *mysag_array)
{
    uint32_t sum = 0,avg = 0;
    for(uint16_t i = 0; i < mysag_array->size; i++)
    {
        sum += mysag_array->data[i];
    }
    avg = sum/mysag_array->size;
    return avg;
}
