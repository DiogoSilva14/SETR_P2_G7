#include "vInit.h"
#include <stdint.h>
#include <stdio.h>

void main(){
    uint32_t array[10];
    vInit(array, 10);

    for(int i=0; i < 10; i++){
        printf("%d \n", array[i]);
    }

    return 0;
}