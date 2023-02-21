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

typedef enum {RUNNING, READY, EXITED, BLOCKED} uthread_state; // state information
queue_t queue;
queue_t dead_queue;

// we store the context, stack and state
struct uthread_tcb 
{
	uthread_ctx_t *context;
	void* stack;
	uthread_state state;
};

// stores information about our current thread
struct uthread_tcb* current_thread; 

struct uthread_tcb *uthread_current(void)
{
	return current_thread;
}

void uthread_yield(void)
{
	// save current thread and make it ready if its running
	struct uthread_tcb* cur_save = uthread_current();
	// cursave is idle initially

	// checking if it is running
	if (cur_save->state == RUNNING)
		cur_save->state = READY;

	struct uthread_tcb *next_thread;

	preempt_disable();
	queue_dequeue(queue, (void**) &next_thread); // dequeue into next_thread
	preempt_enable();

	// make it running now i.e thread 2 is now running
	next_thread->state = RUNNING;

	// now the current thread is the new thread from the queue
	current_thread = next_thread;

	// below function makes cursave ready and push into the queue at the end
	if (cur_save->state == READY)
	{
		preempt_disable();		
		queue_enqueue(queue, cur_save);
		preempt_enable();
	}

	// switch process to next_thread ie thread 2
	uthread_ctx_switch(cur_save->context, next_thread->context);

}

void uthread_exit(void)
{
	struct uthread_tcb *cur_running = uthread_current();

	// frees the current thread's contexts and stack and changes the state to exited	
	cur_running->state = EXITED;
	// push into exited queue
	queue_enqueue(dead_queue, cur_running);
	// goto next thread in queue
	uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb* new_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb)); // initiallize a new_thread

	// initialize new_thread properties
	preempt_disable();
	new_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	new_thread->state   = READY;
	new_thread->stack   = uthread_ctx_alloc_stack();
	preempt_enable();

	// initialize new_thread execution context
	uthread_ctx_init(new_thread->context, new_thread->stack, func, arg);

	/*
	This is a critical section, and is protected by preemption to make sure queue_enqueue, cannot be interrupted to make sure that 2 processes are not enqueued together
	*/	
	preempt_enable();
	queue_enqueue(queue, new_thread);	
	preempt_disable();

	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	// This function starts preemption if true, else returns from it
	preempt_start(preempt); 	

	preempt_disable();
	/*
		This is a critical section, and is protected by preemption to make sure queue_enqueue 
	*/
	queue = queue_create();		
	preempt_enable();

	struct uthread_tcb* idle_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));

	// initialize thread properties and assign our current thread. (This is usually our main thread)
	preempt_disable();
	idle_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	idle_thread->state   = RUNNING;
	preempt_enable();
	current_thread = idle_thread;

	// Initiallizes a new thread with func
	uthread_create(func, arg);

	// Complete all threads before returning to main thread
	while(queue_length(queue) != 0) 
	{
		uthread_yield();

		preempt_disable();
		if(queue_length(dead_queue) > 0)
		{
			struct uthread_tcb *dead_thread;
			while(queue_length(dead_queue) != 0) 
			{
				queue_dequeue(dead_queue, (void**)&dead_thread);
				free(dead_thread->context);
				free(dead_thread->stack);
			}
		}
		preempt_enable();
	}

	preempt_disable();
	/*
		This is a critical section, and is protected by preemption to make sure queue_enqueue 
	*/
	queue_destroy(queue);	
	preempt_enable();

	preempt_stop();   // stop preemption
	return 0;
}

void uthread_block(void)
{
	preempt_disable();
	current_thread->state = BLOCKED;   // block the current thread and yield to the rest
	preempt_enable();
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
	preempt_disable();
	uthread->state = READY;
	preempt_enable();
	
	// enqueue back into the queue 
	preempt_disable();
	/*
		This is a critical section, and is protected by preemption to make sure queue_enqueue 
	*/
	queue_enqueue(queue, uthread);
	preempt_enable();
}

