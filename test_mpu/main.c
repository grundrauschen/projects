#include <stdio.h>
#include <sched.h>
#include <tcb.h>
#include <memmgmt.h>
#include <stddef.h>
#include <cpu.h>
#include <mpu.h>

int main(void)
{
	volatile tcb_t *thread = active_thread;
    puts("Hello World!\n");
    printf("Start Address of thread: %#010x\n", thread);
    printf("Start Address of stack: %#010x\n", thread->memory->start_address);
    printf("Size of the Stack: %d\n", thread->stack_size );
    printf("Controll-Register: %#010x\n", __get_CONTROL());


	enable_unprivileged_mode();

/*	uint32_t *pointer = thread->memory->start_address + 50;
	printf("Writing to Pointer: %#010x\n", pointer);
	*pointer = 0xAAAA;
	printf("Pointer Value: %#010x\n", *pointer);


	printf("TCB Endaddress: %#010x\n\n", thread->memory->end_address);

	uint32_t *pointer = thread->memory->start_address + 40;
	int i = 0;
	for (i=0; i < 1000; i++){
	printf("Writing to Pointer: %#010x\n", pointer);
	*pointer = 0xAAAAAAAA;
	printf("Pointer Value: %#010x\n", *pointer);
	pointer = pointer + 10;
	}

	pointer = thread->memory->start_address - 1;
	printf("Writing to Pointer: %#010x\n", pointer);
	*pointer = 0xAAAA;
	printf("Pointer Value: %#010x\n", *pointer); */

    while(1);
}
