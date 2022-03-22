#include "MySAG_lib.h"

int32_t MySAGInit(MySAG* mysag_array){
    for(int i=0; i < MAX_SIZE; i++){
        mysag_array->data[i] = 0;
    }

    mysag_array->size = 0;

    return 0;
}