#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

// We implement the queue using a LL for all operations to be in O(1) 
// and to navigate forwards and backwards in the queue.

// queue_t is a pointer to an object of struct queue type

// Linked List Node

// Started off with a linked list, changed it to a double linked list for delete operation.
struct Node {
	void* data;
	struct Node* next;
	struct Node* prev;
};


// The LL to store the queue
// Using rear pointer to insert at the end 
struct queue {
	/* TODO Phase 1 */
	struct Node* front;
	struct Node* rear;
	int len;
};


int queueEmpty(queue_t queue){
	return queue->len == 0;
}

void print_queue(queue_t queue){
	if(!queue) {
		// printf("no queue pointer\n");
		return;
	}
	if(queue->len == 0) {
		// printf("queue empty nothing to print\n");
		return;
	}
	struct Node* temp;
	int i = 1;
	temp = queue->front;
	while(temp!=NULL)
	{
		// printf("Element %d is %d \n",i, *(int*) temp->data);
		temp = temp->next;
		i++;
	}

}
queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t q = (queue_t)malloc(sizeof(struct queue));
    q->front = NULL;
	q->rear = NULL;
	q->len = 0;
	// printf("queue created\n\n");
    return q;
}

int queue_destroy(queue_t queue)
{
	if(!queue || queue->len > 0 ) {
		
		return -1;
	} 
	if(queue->len == 0) {
		// free(queue->front);
		free(queue);
		queue = NULL;
		// printf("queue freed\n");
	}
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{

	if ( !queue || !data) {
		// printf("queue or data empty\n");
		return -1;
	}


	// create new node
	struct Node* newNode = malloc(sizeof(struct Node));

	// check for any malloc error
	if(!newNode) 
	{
		// printf("malloc error\n");
		return -1;
	}
	// printf("seg fault not here 2\n");
	
	// assign value 
	newNode->data = data;
	// printf("seg fault not here 3\n");
	//printf("data: %p\n",newNode->data);

	if (queueEmpty(queue)) {
		queue->front = newNode;
		queue->front->next = NULL;
		queue->front->prev = NULL;
		queue->rear = queue->front;
		// queue->rear->next = NULL;
		// queue->rear->prev = NULL;
	}
	else {
		struct Node* tmp = queue->rear;
		queue->rear->next = newNode;
		queue->rear = newNode;

		queue->rear->prev = tmp;
		queue->rear->next = NULL;
	} 

	// printf("enqueue done\n\n");
	// printf("front: %p\n", queue->front->data);
	
	queue->len++;
	// printf("enqueue: Element is %p \n", queue->rear->data);
	// if(queue->rear->next == NULL) {
	// 	printf("next is null\n");
	// }
	print_queue(queue);
	return 0;
}

// @TRISHNA IMPLEMENT THIS

int queue_dequeue(queue_t queue, void **data)
{
	//printf("in queue dequeue\n");
	if(queueEmpty(queue) || !queue || data == NULL) 
	{
		// printf("queue or data NULL; return -1\n");
		return -1;
	}
	if(queue->front == NULL) {

		// printf("aaa\n");
	}

	//printf("data at the front of queue is: %p\n", queue->front->data);
	*data = queue->front->data;
	// *data = &temp_data;
	// printf("data popped is: %p\n", (void**)data);

	struct Node* temp = queue->front;

	if(queue->len == 1) {
		queue->front = NULL;
		queue->rear = NULL;
		queue->len = 0;

	} else {

		queue->front = queue->front->next;
		queue->front->prev = NULL;
		temp->next = NULL;
		queue->len--;
	}
	
	free(temp);
	
	// print_queue(queue);

	//printf("new queue len : %d\n", queue->len);
	// printf("deque done\n\n");
	return 0;
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	int flag = 0, i = 1; // flag to check if data is found

	if(!data || !queue)
		return -1;

	struct Node* temp = queue->front;

	// if queue has only one element

	if((temp->next == NULL) && (temp->data == data)){
		flag = 1;
		queue->rear = NULL;
		queue->front = NULL;
		queue->len--;
		free(temp);
		// printf("deleted last element in queue\n");
		// print_queue(queue);
		return 0;
	}

	// printf("\ndata: %p\n", (void*)data);

	while(temp!=NULL){
		// printf("temp data: %p\n", temp->data);

		if (temp->data == data)
		{
			
			flag = 1;
			struct Node *P = temp->prev;
			struct Node *N = temp->next;
			if(i == 1) {
				queue->front = N;
				N->prev = NULL;
			} 
			else if(i == queue->len) {
				queue->rear = P;
				P->next = NULL;
			} else {
				N->prev = P;
				P->next = N;
			}
			free(temp);
			// printf("delete completed\n\n");
			break;
		}
		temp = temp->next;
		i++;
	}
		
	queue->len--;
	// print_queue(queue);
	if(flag == 1)
		return 0;
	return -1;
	
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	if(!queue || !func)
		return -1;
	
	// iterate through queue

	struct Node* temp;
	temp = queue->front;
	struct Node* temp1 = NULL;
	while(temp!=NULL){
		temp1 = temp->next;
		func(queue, temp->data);
		// temp = NULL;
		temp = temp1;
	}

	return 0;

}

int queue_length(queue_t queue)
{
	//printf("Checking queue len \n");
	int length = queue->len;
	//printf("queue len : %d\n",length);
	if(queue) {
		//printf("yes\n");
	} else {
		//printf("no\n");
		return -1;
	}
	/* TODO Phase 1 */
	return length;
}

/* tester function
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}
*/


// int main()
// {
// 	//printf("doing queue_create:\n");
// 	int data = 3, *ptr;
// 	queue_t q;
// 	q = queue_create();
// 	queue_enqueue(q, &data);
// 	queue_dequeue(q, (void**)&ptr);
// 	printf("data out: %d\n", *ptr);

// 	return 0;
// }
