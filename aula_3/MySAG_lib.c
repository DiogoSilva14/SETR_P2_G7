#include "MySAG_lib.h"
#include <stdbool.h>

uint32_t mysag_max(MySAG *mysag){
    uint32_t temp=0;
    
    for (uint16_t i=0; i < (mysag->size); i++){
        if (mysag->data[i]>temp)
            temp = mysag->data[i];
    }

    return temp;
}

