#include <stdio.h>

int main(){

    init_sm();

    while(1){
        sm_loop();
    }

    return 0;
}