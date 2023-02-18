#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
	size_t semaphore_count;
	queue_t blocked_list;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t new_semaphore = malloc(sizeof(struct semaphore));
	if(!new_semaphore)
		return NULL;
	new_semaphore->blocked_list = queue_create();
	new_semaphore->semaphore_count = count;
	return new_semaphore;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if (!sem) return -1;
	
	if (queue_destroy(sem->wait_list) == -1) return -1;
	preempt_enable();
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
}

