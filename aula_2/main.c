#include "vInit.h"
#include "vAdd.h"
#include <stdint.h>
#include <stdio.h>

void main(){
    uint32_t array[10];
    vInit(array, 10);
    uint32_t sum = vAdd(array,10);

    for(int i=0; i < 10; i++){
        printf("%d \n", array[i]);
    }

    printf("Sum = %d\n", sum);


    return 0;
}