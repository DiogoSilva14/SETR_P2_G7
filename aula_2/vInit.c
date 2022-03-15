#include "vInit.h"

void vInit(uint32_t* vector, uint32_t size){
    for(int i=0; i < size; i++){
        vector[i] = i+1;
    }
}