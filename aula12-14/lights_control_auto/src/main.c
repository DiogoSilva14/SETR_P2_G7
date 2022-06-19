#include <stdlib.h>
#include <stdio.h>
#include "device_drivers.h"
    
/* Thread code prototypes */
void thread_A_code(void *argA, void *argB, void *argC);
void thread_B_code(void *argA, void *argB, void *argC);
void thread_C_code(void *argA, void *argB, void *argC);

void main(void){
	init_drivers();

	sem_init(SEM_AA, 1, 1);
	sem_init(SEM_AB, 0, 1);
	sem_init(SEM_BC, 0, 1);
    
	thread_create(THREAD_A, thread_A_code);
	thread_create(THREAD_B, thread_B_code);
	thread_create(THREAD_C, thread_C_code);	
}

/* Thread code implementation */
void thread_A_code(void *argA , void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;

    /* Compute next release instant */
    release_time = get_uptime() + thread_A_period;

    /* Thread loop */
    while(1) {
		
		printf("THREAD A \n\r");

		sem_give(SEM_AB);      

        /* Wait for next release instant */ 
        fin_time = get_uptime();
		
        if(fin_time < release_time) {
            sleep_ms(release_time - fin_time);
            release_time += thread_A_period;
        }
    }
}

void thread_B_code(void *argA , void *argB, void *argC){
    while(1) {
		sem_take(SEM_AB);
        
		printf("THREAD B \n\r");

		sem_give(SEM_BC);     
  }
}

void thread_C_code(void *argA , void *argB, void *argC){
	/* Thread loop */
    while(1) {
		sem_take(SEM_BC);

		printf("THREAD C \n\r");

    }
}