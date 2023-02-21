#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	size_t semaphore_count;
	queue_t blocked_list;
};

sem_t sem_create(size_t count)
{
	preempt_disable();
	sem_t new_semaphore = malloc(sizeof(struct semaphore));
	if(!new_semaphore)							
		// return NULL if unable to allocate memory
		return NULL;

	new_semaphore->blocked_list = queue_create();	
	if(!new_semaphore->blocked_list)
		// return NULL if unable to allocate memory
		return NULL;	

	new_semaphore->semaphore_count = count;
	preempt_enable();

	return new_semaphore;
}

int sem_destroy(sem_t sem)
{
	if (!sem) return -1;
	preempt_disable();
	if (queue_destroy(sem->blocked_list) == -1)  
		// destroy the semaphore as explained
		return -1;
	preempt_enable();
	free(sem);
	return 0;
}


int sem_down(sem_t sem)
{
	if (!sem) return -1;

	if (sem->semaphore_count == 0) 
	{
		struct uthread_tcb *cur_tcb = uthread_current();	
		// if no resources are available, push the thread to the blocked threads queue

		preempt_disable();
		queue_enqueue(sem->blocked_list, cur_tcb);
		preempt_enable();

		uthread_block();	// block the thread
	} 
	else sem->semaphore_count--;
	return 0;
}

int sem_up(sem_t sem)
{
	if (!sem) return -1;
	if (queue_length(sem->blocked_list) != 0) 
	{		
		// if there are threads being blocked, release resource to them
		struct uthread_tcb *front;

		preempt_disable();
		int success = queue_dequeue(sem->blocked_list, (void**) &front); // if dequeue fails, return -1
		preempt_enable();
		
		if (success == -1)
			return -1;
		uthread_unblock(front);
	}
	else sem->semaphore_count++;
	return 0;
}

