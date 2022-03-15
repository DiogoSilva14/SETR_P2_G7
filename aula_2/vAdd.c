
#include  <stdio.h>
#include  <stdlib.h>


uint32_t vAdd(uint32_t *num_v, uint32_t size){
    uint32_t V_sum=0;
    for (int i = 0; i<size; i++){
        V_sum+= num_v[i];
    }

    return V_sum;

}

