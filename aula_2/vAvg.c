#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t vAvg(uint32_t *vector, uint32_t size)
{
    uint32_t sum = 0;
    sum = vAdd(vector, size);
    //for(uint32_t i=0;i<size;i++)
    //{
    //    sum += vector[i];
    //}
    return sum/size;
}
