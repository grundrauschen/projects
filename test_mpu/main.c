#include <stdio.h>
#include <sched.h>
#include <tcb.h>

int main(void)
{
	volatile tcb_t *thread = active_thread;
    puts("Hello World!\n");
    printf("Start Address of thread: %#010x\n", thread);
    printf("Start Address of stack: %#010x\n", thread->stack_start);
    printf("Size of the Stack: %d\n", thread->stack_size );

    uint32_t writesize = thread->stack_size /2;
    uint8_t *writepoint;
    writepoint = thread->stack_start;
    for (uint32_t i=0; i<= writesize; i++) {
    	*writepoint = 0xAA;
    	writepoint++;
    }

    while(1);
}
