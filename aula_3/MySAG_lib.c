#include "MySAG_lib.h"

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