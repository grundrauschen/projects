#include <stdio.h>
#include <sched.h>
#include <tcb.h>
#include <memmgmt.h>
#include <stddef.h>
#include <cpu.h>
#include <mpu.h>
#include <thread.h>
#include "main.h"

unsigned int testcase = 0;

const char *thread1_name = "thread1";
const char *tmur1 = "test_msg_underpriviliged_reciever: receiver";
const char *tmur2 = "test_msg_underpriviliged_reciever: sender";
const char *tmur3 = "test_msg_underpriviliged_reciever_payload: receiver";
const char *tmur4 = "test_msg_underpriviliged_reciever_payload: sender";
const char *tmur5 = "test_msg_priviliged_reciever: receiver";
const char *tmur6 = "test_msg_priviliged_reciever: sender";
const char *tmur7 = "test_msg_priviliged_reciever_payload: receiver";
const char *tmur8 = "test_msg_priviliged_reciever_payload: sender";
const char *tmur9 = "test_msg_priviliged_reciever_buffered: receiver";
const char *tmur10 = "test_msg_priviliged_reciever_buffered: sender";

static int pid1;
static int pid2;
static int blocking;
static unsigned int size;
static char *string = "This is a Payload";

void thread1(void){
	puts("thread1!\n");
	//svc_switch_context_exit();
	svc_thread_sleep();
	while(1);
	return;
}

void receiver(void){
	puts("This is Receiver\n");
	while (1){
		msg_t m;
		svc_msg_receive(&m);
		printf("RESC: MSG Value %d\n", m.content.value);
		printf("RESC: MSG Sender PID %d\n", m.sender_pid);
		printf("RESC: MSG Size %d\n", m.size);
		printf("RESC: MSG Type %d\n", m.type);
	}
}

void receiver_buffered(void){
	puts("This is Receiver\n");
	while (1){
		msg_t m[16];
		svc_msg_init_queue(&m, 16);
		svc_thread_sleep();
		svc_msg_receive(&m);
		printf("RESC: MSG Value %d\n", m[0].content.value);
		printf("RESC: MSG Sender PID %d\n", m[0].sender_pid);
		printf("RESC: MSG Size %d\n", m[0].size);
		printf("RESC: MSG Type %d\n", m[0].type);
	}
}

void receiver_payload(void){
	puts("This is Receiver\n");
	while (1){
		msg_t m;
		char payload[size];
		m.size = size;
		m.content.ptr = payload;
		svc_msg_receive(&m);
		printf("RESC: MSG Payload%d\n");
		printf(payload);
		printf("\n");
		printf("RESC: MSG Sender PID %d\n", m.sender_pid);
		printf("RESC: MSG Size %d\n", m.size);
		printf("RESC: MSG Type %d\n", m.type);
	}
}


void sender(void){
	puts("This is Sender\n");
	msg_t m;
	m.content.value = 42;
	m.sender_pid = thread_pid;
	m.size = 0;
	m.type = 5;
	printf("Send: MSG Value %d\n", m.content.value);
	printf("Send: MSG Sender PID %d\n", m.sender_pid);
	printf("Send: MSG Size %d\n", m.size);
	printf("Send: MSG Type %d\n", m.type);
	svc_msg_send(&m, pid1 , blocking);
	svc_thread_sleep();
}

void sender_payload(void){
	puts("This is Sender\n");
	msg_t m;
	m.content.ptr = string;
	m.sender_pid = thread_pid;
	m.size = size;
	m.type = 7;
	printf("Send: MSG Payload%d\n");
	printf(string);
	printf("\n");
	printf("Send: MSG Sender PID %d\n", m.sender_pid);
	printf("Send: MSG Size %d\n", m.size);
	printf("Send: MSG Type %d\n", m.type);
	svc_msg_send(&m, pid1 , blocking);
	svc_thread_sleep();
}


void test_thread_creation(void){
	puts("Hallo testfunk!\n");
	thread_description thread;
	thread.stacksize = 1024;
	thread.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread.priority = 14;
	thread.function = thread1;
	thread.name = thread1_name;
	pid1 = thread_create_desc(&thread);
	printf("PID: %d\n", pid1);
	//thread_create(1024, 13, CREATE_WOUT_YIELD | CREATE_STACKTEST, thread1, thread1_name);
	svc_thread_sleep();
}

void test_msg_priv_reciever_waiting(void){
	puts("Creating Recieve Task\n");
	thread_description thread;
	thread.stacksize = 512;
	thread.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread.priority = 11;
	thread.function = receiver;
	thread.name = tmur5;
	pid1 = thread_create_desc(&thread);
	puts("Creating Send Task\n");
	blocking = 1;
	thread_description thread2;
	thread2.stacksize = 512;
	thread2.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread2.priority = 12;
	thread2.function = sender;
	thread2.name = tmur6;
	pid2 = thread_create_desc(&thread2);
	svc_switch_context_exit();
}

void test_msg_priv_reciever_waiting_buffer(void){
	puts("Creating Recieve Task\n");
	thread_description thread;
	thread.stacksize = 512;
	thread.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread.priority = 11;
	thread.function = receiver_buffered;
	thread.name = tmur9;
	pid1 = thread_create_desc(&thread);
	puts("Creating Send Task\n");
	blocking = 1;
	thread_description thread2;
	thread2.stacksize = 512;
	thread2.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread2.priority = 12;
	thread2.function = sender;
	thread2.name = tmur10;
	pid2 = thread_create_desc(&thread2);
	svc_switch_context_exit();
}

void test_msg_priv_reciever_waiting_payload(void){
	puts("Creating Recieve Task\n");
	thread_description thread;
	thread.stacksize = 512;
	thread.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread.priority = 12;
	thread.function = receiver_payload;
	thread.name = tmur7;
	pid1 = thread_create_desc(&thread);
	puts("Creating Send Task\n");
	blocking = 1;
	thread_description thread2;
	thread2.stacksize = 512;
	thread2.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread2.priority = 12;
	thread2.function = sender_payload;
	thread2.name = tmur8;
	pid2 = thread_create_desc(&thread2);
	svc_switch_context_exit();
}

void test_msg_underpriviliged_reciever(void){
	puts("Creating Recieve Task\n");
	thread_description thread;
	thread.stacksize = 512;
	thread.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread.priority = 12;
	thread.function = receiver;
	thread.name = tmur1;
	pid1 = thread_create_desc(&thread);
	puts("Creating Send Task\n");
	blocking = 1;
	thread_description thread2;
	thread2.stacksize = 512;
	thread2.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread2.priority = 11;
	thread2.function = sender;
	thread2.name = tmur2;
	pid2 = thread_create_desc(&thread2);
	svc_switch_context_exit();
}

void test_msg_underpriviliged_reciever_payload(void){
	puts("Creating Recieve Task\n");
	thread_description thread;
	thread.stacksize = 512;
	thread.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread.priority = 12;
	thread.function = receiver_payload;
	thread.name = tmur3;
	pid1 = thread_create_desc(&thread);
	puts("Creating Send Task\n");
	blocking = 1;
	thread_description thread2;
	thread2.stacksize = 512;
	thread2.flags = CREATE_WOUT_YIELD | CREATE_STACKTEST;
	thread2.priority = 11;
	thread2.function = sender_payload;
	thread2.name = tmur4;
	pid2 = thread_create_desc(&thread2);
	svc_switch_context_exit();
}

int main(void)
{
	volatile tcb_t *thread = active_thread;
    puts("Hello World!\n");
    printf("Start Address of thread: %#010x\n", thread);
    printf("Start Address of stack: %#010x\n", thread->memory->start_address);
    printf("Size of the Stack: %d\n", thread->stack_size );
    printf("Controll-Register: %#010x\n\n", __get_CONTROL());


    testcase = 6;
    printf("Testcase: %d\n\n", testcase);

    enable_unprivileged_mode();

	size = strlen(string);

    switch(testcase){
    	case 0: break;
    	case 1: test_thread_creation();
    			break;
    	case 2: test_msg_underpriviliged_reciever();
    			break;
    	case 3: test_msg_priv_reciever_waiting();
    			break;
    	case 4: test_msg_underpriviliged_reciever_payload();
    			break;
    	case 5: test_msg_priv_reciever_waiting_payload();
    			break;
    	case 6: test_msg_priv_reciever_waiting_buffer();
    			break;
    	default: break;
    }



	//testfunk();

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

    //(1);

    svc_thread_sleep();
}
