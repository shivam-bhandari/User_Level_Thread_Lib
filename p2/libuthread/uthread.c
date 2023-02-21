#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"


// use of preemption before all queue processes, since we don't want queue resources 
// manipulated by multiple threads

typedef enum {RUNNING,READY,EXITED,BLOCKED} uthread_state;
queue_t queue;

struct uthread_tcb {
	/* TODO Phase 2 */
	uthread_ctx_t *context;
	void* stack;
	uthread_state state;
};

struct uthread_tcb* currentThread;

struct uthread_tcb *uthread_current(void)
{
	return currentThread;
}

void uthread_yield(void)
{
	// save current thread and make it ready if its running
	struct uthread_tcb* cur_save = uthread_current();
	// cursave is idle initially

	// just checking if it is running (it should be, but I am just precautionary)
	if (cur_save->state == RUNNING)
		cur_save->state = READY;

	struct uthread_tcb *next_thread;

	preempt_disable();
	queue_dequeue(queue, (void**) &next_thread); // dequeue into next_thread
	preempt_enable();

	// make it running now i.e thread 2 is now running
	next_thread->state = RUNNING;

	// now the current thread is the new thread from the queue
	currentThread = next_thread;

	// push the old thread to back of the queue if it is ready
	// enqueue the old one to the back of the queue
	// make cursave ready and put into the queue at the end

	// check if ready since it can be interrupted
	if (cur_save->state == READY)
	{
		preempt_disable();		
		queue_enqueue(queue, cur_save);
		preempt_enable();
	}

	// switch process to next_thread ie thread 2
	uthread_ctx_switch(cur_save->context, next_thread->context);

	// if (cur_save->state == EXITED) {
	// 	free(cur_save->context);
	// 	free(cur_save->stack);
	// 	free(cur_save);
	// 	cur_save = NULL;
	// }
}

void uthread_exit(void)
{

	struct uthread_tcb *cur_running = uthread_current();

	free(cur_running->context);
	free(cur_running->stack);		// frees the current thread's contexts and stack and changes the state to exited
	cur_running->state = EXITED;
	// goto next thread in queue
	uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb* new_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb)); // initiallize a new_thread

	// initialize new_thread properties
	new_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	new_thread->state   = READY;
	new_thread->stack   = uthread_ctx_alloc_stack();

	// initialize new_thread execution context
	uthread_ctx_init(new_thread->context, new_thread->stack, func, arg);

	preempt_enable();
	queue_enqueue(queue, new_thread);	//	This is a critical section, and is protected by preemption to make sure queue_enqueue cannot be interrupted to make sure that 2 processes are not enqueued together
	preempt_disable();
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	preempt_start(preempt); 	//This function starts preemption if true, else returns from it

	preempt_disable();
	queue = queue_create();		// This is a critical section, and is protected by preemption to make sure queue_create cannot be interrupted
	preempt_enable();

	struct uthread_tcb* idle_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));

	// initialize thread properties and assign our current thread. (This is usually our main thread)
	idle_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	idle_thread->state   = RUNNING;
	currentThread = idle_thread;

	// Initiallizes a new thread with func
	uthread_create(func, arg);

	// Complete all threads before returning to main thread
	while(queue_length(queue) != 0) 
		uthread_yield();

	

	preempt_disable();
	queue_destroy(queue);	// This is a critical section, and is protected by preemption to make sure queue_destroy cannot be interrupted
	preempt_enable();

	preempt_stop();   // stop preemption
	return 0;
}

void uthread_block(void)
{
	currentThread->state = BLOCKED;   // block the current thread and yield to the rest
	uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	// check if blocked, if not blocked return error
	if(uthread->state != BLOCKED)
	{
		printf("Thread is not blocked\n");
		return;
	}
	// Change state to ready and put it in the queue for running
	uthread->state = READY;
	// enqueue back into the queue 
	preempt_disable();
	queue_enqueue(queue, uthread);
	preempt_enable();
}

