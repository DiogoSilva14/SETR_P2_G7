#include "MySAG_lib.h"
#include <stdbool.h>

uint32_t MySagAvg(MySAG *mysag_array)
{
    uint32_t sum = 0,avg = 0;
    for(uint16_t i = 0; i < mysag_array->size; i++)
    {
        sum += mysag_array->data[i];
    }
    avg = sum/mysag_array->size;
    return avg;
}

int32_t MySAGInit(MySAG* mysag_array){
    for(int i=0; i < MAX_SIZE; i++){
        mysag_array->data[i] = 0;
    }

    mysag_array->size = 0;

    return 0;
}
