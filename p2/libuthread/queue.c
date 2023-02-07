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

void print_queue(queue_t queue){
	// if(!queue)
	// 	break;
	
	struct Node* temp;
	int i = 1;
	temp = queue->front;
	while(temp!=NULL)
	{
		printf("Element %d is %p \n",i, temp->data);
		temp = temp->next;
		i++;
	}
	i--;
	temp = queue->rear;

	while(temp!=NULL)
	{
		printf("Element %d is %p \n",i, temp->data);
		temp = temp->prev;
		i--;
	}
}

int queueEmpty(queue_t queue){
	return queue->len == 0;
}

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t q = (queue_t)malloc(sizeof(struct queue));
    q->front = NULL;
	q->rear = NULL;
	q->len = 0;
	printf("queue created\n\n");
    return q;
}

// int queue_destroy(queue_t queue)
// {
// 	/* TODO Phase 1 */
// 	if (queueEmpty(queue) || !queue){
// 		return -1;
// 	}

// 	int* tmp_data = 0;
// 	while(!queueEmpty(queue)) {
// 		free(queue->front); // IMPLEMENT FUNCTION
// 		queue_dequeue(queue, (void *)tmp_data);
// 	}
// 	return 0;
// }

int queue_enqueue(queue_t queue, void *data)
{
	//printf("in queue enqueue\n");
	/* TODO Phase 1 */


	if ( !queue || !data) {
		printf("queue or data empty\n");
		return -1;
	}


	// create new node
	struct Node* newNode = malloc(sizeof(struct Node));

	// check for any malloc error
	if(!newNode) 
	{
		printf("malloc error\n");
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
		queue->rear = newNode;
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

	printf("enqueue done\n\n");
	// printf("front: %p\n", queue->front->data);
	
	queue->len++;
	//print_queue(queue);
	return 0;
}

// @TRISHNA IMPLEMENT THIS

int queue_dequeue(queue_t queue, void **data)
{
	//printf("in queue dequeue\n");
	if(queueEmpty(queue) || !queue || data == NULL) 
	{
		printf("queue or data NULL; return -1\n");
		return -1;
	}
	if(queue->front == NULL) {

		printf("aaa\n");
	}

	//printf("data at the front of queue is: %p\n", queue->front->data);
	void** temp_data = queue->front->data;
	data = temp_data;
	//printf("data popped is: %p\n", (void**)data);

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
	
	print_queue(queue);

	//printf("new queue len : %d\n", queue->len);
	printf("deque done\n\n");
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
		printf("deleted last element in queue\n");
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
			printf("delete completed\n\n");
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
	
	// // iterate through queue

	struct Node* temp;
	temp = queue->front;
	struct Node* tempt = NULL;
	while(temp!=NULL){
		tempt = temp->next;

		func(queue, temp->data);
		temp = NULL;
		temp = tempt;
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

// function to print queue
 

int main()
{
	//printf("doing queue_create:\n");
	struct queue* q = queue_create();
	int no = 1;
	int *temp = &no;
	no = 30;
	queue_enqueue(q, (void*)30);
	queue_enqueue(q, (void*)40);
	queue_enqueue(q, (void*)50);

	queue_delete(q,(void*)40);
	queue_delete(q,(void*)50);
	queue_delete(q,(void*)30);

	print_queue(q);
	printf("\nfinal queue length: %d\n", queue_length(q));

	return 0;
}