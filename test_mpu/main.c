#include <stdio.h>
#include <sched.h>
#include <tcb.h>
#include <memmgmt.h>

int main(void)
{
	volatile tcb_t *thread = active_thread;
    puts("Hello World!\n");
    printf("Start Address of thread: %#010x\n", thread);
    printf("Start Address of stack: %#010x\n", thread->memory->start_address);
    printf("Size of the Stack: %d\n", thread->stack_size );
    printf("Controll-Register: %#010x\n", __get_CONTROL());

    uint32_t writesize = 64;
    uint8_t *writepoint;
    writepoint = thread->memory->start_address;
	enable_unprivileged_mode();
    for (uint32_t i=0; i<= writesize; i++) {
    	*writepoint = 0xAA;
    	writepoint++;
    }

    while(1);
}
