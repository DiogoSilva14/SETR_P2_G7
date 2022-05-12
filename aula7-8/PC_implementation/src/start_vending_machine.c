#include <stdio.h>
#include "state_machine.h"

int main(){

    if(init_sm() < 0){
        return -1;
    }

    while(1){
        sm_loop();
    }

    return 0;
}