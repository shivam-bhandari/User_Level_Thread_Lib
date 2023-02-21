#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

/*
	Creation of a doubly linked-list having a pointer to the next and prev node element.

	data: data in the node element
	next: pointer to the next element in the linked list
	prev: pointer to the previous element in the linked list
*/
struct Node {
	void* data;
	struct Node* next;
	struct Node* prev;
};

/* 
	Struct for the queue. It is implemented using linked list. It has a pointer to the front of the queue, pointer to the back of the queue and the length of the queue.

	front: oldest element of queue
	rear: most latest element added to the queue
	len: number of elements in the queue
*/
struct queue {
	struct Node* front;
	struct Node* rear;
	int len;
};

int queueEmpty(queue_t queue){
	return queue->len == 0;
}

/*
	Helper function to the print the contents of the queue for debugging purposes.
*/
void print_queue(queue_t queue){
	struct Node* temp;
	int i = 1;
	temp = queue->front;
	while(temp!=NULL)
	{
		printf("Element %d is %d \n",i, *(int*)temp->data);
		temp = temp->next;
		i++;
	}
	
	printf("\n");
}


queue_t queue_create(void)
{
	queue_t q = (queue_t)malloc(sizeof(struct queue));
    q->front = NULL;
	q->rear = NULL;
	q->len = 0;
    return q;
}

int queue_destroy(queue_t queue)
{
	if(!queue || queue->len > 0 ) {
		return -1;
	} 
	if(queue->len == 0) {
		free(queue);
		queue = NULL;
	}
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{

	if ( !queue || !data) {
		return -1;
	}

	struct Node* newNode = malloc(sizeof(struct Node));

	if(!newNode) 
	{
		return -1;
	}
	newNode->data = data;

	if (queueEmpty(queue)) {
		queue->front = newNode;
		queue->front->next = NULL;
		queue->front->prev = NULL;
		queue->rear = newNode;
	}
	else {
		struct Node* tmp = queue->rear;
		queue->rear->next = newNode;
		queue->rear = newNode;

		queue->rear->prev = tmp;
		queue->rear->next = NULL;
	} 
	queue->len++;
	return 0;
}



int queue_dequeue(queue_t queue, void **data)
{
	if(queueEmpty(queue) || !queue || data == NULL) 
	{
		return -1;
	}

	*data = queue->front->data;

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
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	int flag = 0, i = 1;

	if(!data || !queue )
		return -1;
	if(queue_length(queue) == 0) {
		return -1;
	}

	struct Node* temp = queue->front;


	if((temp->next == NULL) && (temp->data == data)){
		flag = 1;
		queue->rear = NULL;
		queue->front = NULL;
		queue->len--;
		free(temp);
		return 0;
	}


	while(temp!=NULL){
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
			queue->len--;
			break;
		}
		temp = temp->next;
		i++;
	}
	if(flag == 1){
		return 0;
	}
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(!queue || !func)
		return -1;
	
	// iterate through queue
	// keeping temp1 since we dont want to lose the pointer if delete is called on an element
	struct Node* temp;
	temp = queue->front;
	struct Node* temp1 = NULL;
	while(temp!=NULL){
		temp1 = temp->next;
		func(queue, temp->data);
		temp = temp1;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	int length = queue->len;
	if(!queue) {
		return -1;
	}
	return length;
}
