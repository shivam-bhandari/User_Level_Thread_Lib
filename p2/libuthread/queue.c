#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

// We implement the queue using a LL for all operations to be in O(1) 
// and to navigate forwards and backwards in the queue.

// queue_t is a pointer to an object of struct queue type

// Linked List Node
struct Node {
	void* data;
	struct Node* next;
}


// The LL to store the queue
// Using rear pointer to insert at the end 
struct queue {
	/* TODO Phase 1 */
	struct Node* front;
	struct Node* rear;
	int len;
};

bool queueEmpty(queue_t queue){
	return queue_length(queue) == 0;
}

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->front = q->rear = NULL;
	q->len = 0;
    return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if (queueEmpty){
		return -1;
	}
	while(!queueEmpty(queue)) {
		free(queueFront(queue));
		queuePop(queue);
	}
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if ( !data || !queue) 
		return -1;

	// create new node, and assign data
	Node* newNode = malloc(sizeof(Node));
	newNode->data = data;

	if(!newNode)
		return -1

	if (queueEmpty(queue)) {
		queue->rear = NULL;
		queue->front = newNode;
		newNode->next = newNode->prev = NULL;
	}

	else {
		queue->rear->next = newNode;
		newNode->prev = queue->rear;
		queue->rear = newNode;
		newNode->next = NULL;
	} 

	queue->size = queue->size + 1;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

