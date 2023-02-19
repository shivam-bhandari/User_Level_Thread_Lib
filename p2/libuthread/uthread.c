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

typedef enum {RUNNING,READY,EXITED,BLOCKED}uthread_state;
queue_t queue;

struct uthread_tcb {
	/* TODO Phase 2 */
	uthread_ctx_t *context;
	void* stack;
	int id;
	uthread_state state;
};

int thread_id = 0;
struct uthread_tcb* currentThread;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return currentThread;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	// save current thread and make it ready if its running
	struct uthread_tcb* cur_save = uthread_current();
	// cursave is idle initially
	if (cur_save->state == RUNNING)
		cur_save->state = READY;

	struct uthread_tcb *front;

	queue_dequeue(queue, (void**) &front);

	// make it running now i.e thread 2 is now running
	front->state = RUNNING;

	// now the current thread is the new thread from the queue
	currentThread = front;

	// push the old thread to back of the queue if it is ready
	// enqueue the old one to the back of the queue
	// make cursave ready and put into the queue at the end
	if (cur_save->state == READY)
	{		
		queue_enqueue(queue, cur_save);
	}

	// switch process to front ie thread 2
	uthread_ctx_switch(cur_save->context, front->context);

	if (cur_save->state == EXITED) {
		free(cur_save->context);
		free(cur_save->stack);
		free(cur_save);
		cur_save = NULL;
	}
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	struct uthread_tcb *cur_running = uthread_current();

	free(cur_running->context);
	free(cur_running->stack);
	cur_running->state = EXITED;

	// goto next thread in queue
	uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	struct uthread_tcb* thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));

	// initialize thread properties
	thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	thread->state   = READY;
	thread->id 		= thread_id++;
	thread->stack   = uthread_ctx_alloc_stack();

	// initialize thread execution context
	uthread_ctx_init(thread->context, thread->stack, func, arg);
	queue_enqueue(queue, thread);
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	queue = queue_create();
	struct uthread_tcb* idle_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));

	// initialize thread properties
	idle_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	idle_thread->state   = RUNNING;
	idle_thread->id      = thread_id;
	thread_id++;

	// set current thread to
	currentThread = idle_thread;

	uthread_create(func, arg);

	// set idle 
	while(queue_length(queue) != 0) 
		uthread_yield();

	queue_destroy(queue);
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
	currentThread->state = BLOCKED;
	uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
	// check if blocked
	assert(uthread->state == BLOCKED);

	// find the thread in the queue and change state
	uthread->state = READY;
	
	// enqueue back into the queue 
	queue_enqueue(queue, uthread);
}

