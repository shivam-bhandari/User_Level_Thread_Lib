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
	return queue->len == 0;
}

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t q = (queue_t)malloc(sizeof(struct queue));
    q->front = NULL;
	q->rear = NULL;
	q->len = 0;
    return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if (queueEmpty(queue) || !queue){
		return -1;
	}
	while(!queueEmpty(queue)) {
		free(FRONTOFQUEUE(queue)); // IMPLEMENT FUNCTION
		queue_dequeue(queue);
	}
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if ( !data || !queue) 
		return -1;

	// create new node
	Node* newNode = malloc(sizeof(Node));

	// check for any malloc error
	if(!newNode) 
	{
		printf("malloc error\n");
		return -1;
	}
	
	// assign value 
	newNode->data = data;


	if (queueEmpty(queue)) {
		queue->rear = NULL;
		queue->front = newNode;
		newNode->next = NULL;
	}
	else {
		queue->rear->next = newNode;
		queue->rear = queue->rear->next;
		queue->rear->next = NULL;
	} 

	queue->len++;
	return 0;
}

// @TRISHNA IMPLEMENT THIS

int queue_dequeue(queue_t queue, void **data)
{
	if(!queue || **data == NULL) 
	{
		printf("queue or data NULL; return -1\n")
		return -1;
	}
	if(front exists) {
		
	}
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	int flag = 0; // flag to check if data is found

	if(!data || !queue)
		return -1;

	Node* temp = queue->front;

	// if queue has only one element

	if(temp->next == NULL && && temp->data == data){
		flag = 1;
		queue->back = NULL;
		queue->front = NULL;
		free(temp);
		return 0;
	}

	while(temp!=NULL){

		if (temp-> data == data)
		{
			flag = 1;
			node *T = temp;
			temp->prev->next = T->next;

			free(T);
			break;
		}
		temp = temp->next;
	}
		
	queue->size--;
	if(flag == 0)
		return flag;
	return -1;
	
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	// if(!queue || !func)
	// 	return -1;
	
	// // iterate through queue



}

int queue_length(queue_t queue)
{
	if(!queue)
		return -1;
	/* TODO Phase 1 */
	return queue->len;
}

