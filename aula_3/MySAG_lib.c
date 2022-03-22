#include "MySAG_lib.h"

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

uint32_t MySAGMax(MySAG *mysag){
    uint32_t temp=0;
    
    for (uint16_t i=0; i < (mysag->size); i++){
        if (mysag->data[i]>temp)
            temp = mysag->data[i];
    }

    return temp;
}

uint32_t MySAGMin(MySAG *mysag){
    uint32_t temp=mysag->data[1];
    
    for (uint16_t i=0; i < (mysag->size); i++){
        if (mysag->data[i]<temp)
            temp = mysag->data[i];
    }

    return temp;
}

int32_t MySAGInit(MySAG* mysag_array){
    for(int i=0; i < MAX_SIZE; i++){
        mysag_array->data[i] = 0;
    }

    mysag_array->size = 0;

    return 0;
}

int32_t MySAGInsert(MySAG* mysag_array, uint32_t value){
    if(mysag_array->size == 100){
        printf("MySAG is full\n");
        return -1;
    }

    mysag_array->data[mysag_array->size] = value;
    
    mysag_array->size++;

    return 0;
}
