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

	while(temp != NULL)
	{
		printf("Element %d is %d \n",i, *(int*)temp->data);
		temp = temp->next;
		i++;
	}	
	printf("\n");
}

/*
	Initializes the queue. Pointer to the front is NULL, pointer to the rear is also NULL. This is because there are no elements present in the queue when we first create.

	The length of the queue is 0.
*/
queue_t queue_create(void)
{
	queue_t q = (queue_t)malloc(sizeof(struct queue));
    q->front = NULL;
	q->rear = NULL;
	q->len = 0;
    return q;
}

/*
	Destroys an empty queue. Only destroys the pointer pointiong to the queue, by freeing up the space allocated to the pointer. Doesnt free up space for any elements in the doubly linked list.

	Time Complexity: O(1)
*/
int queue_destroy(queue_t queue)
{
	if(!queue || queue->len > 0) {
		return -1;
	} 
	if(queue->len == 0) {
		free(queue);
		queue = NULL;
	}
	return 0;
}

/*
	Time Complexity: O(1)
*/
int queue_enqueue(queue_t queue, void *data)
{
	if (!queue || !data) {
		return -1;
	}

	struct Node* newNode = malloc(sizeof(struct Node));
	
	/* Error handler for malloc error*/
	if(!newNode) 
	{
		return -1;
	}
	newNode->data = data;

	if (queueEmpty(queue)) { 
		/* Adding the first element to the queue or adding to the queue when the queue has been empty */
		queue->front = newNode;
		queue->front->next = NULL;
		queue->front->prev = NULL;
		queue->rear = newNode;
		queue->rear->next = NULL;
		queue->rear->prev = NULL;
	}
	else {
		/* Adding element when the queue has other elements */
		struct Node* tmp = queue->rear; // temporary node to hold the rear element
		queue->rear->next = newNode; // assigns the next pointer of the rear node to the newNode
		queue->rear = newNode; // the rear node now points to this newly added node
		queue->rear->prev = tmp; // the prev node of the rear points to the tmp node that contained the previous rear node
		queue->rear->next = NULL;
	} 
	queue->len++; // increments the length of the queue
	return 0;
}

/*
	Time Complexity: O(1)
*/
int queue_dequeue(queue_t queue, void **data)
{
	if(!queue || !data) {
		return -1;
	}
	if(queueEmpty(queue)) 
	{
		return -1;
	}
	/* Gets the data of the oldest element (at the front of the queue)*/
	*data = queue->front->data;

	/* temporary node to hold the front of the queue to free the space after de-linking it from the queue*/
	struct Node* temp = queue->front; 

	if(queue->len == 1) {
		/* If theres only one element in the queue that needs to be dequeued, the queue now essentially becomes an empty queue. So the front and rear of the queue point to null and the length is 0.*/
		queue->front = NULL;
		queue->rear = NULL;
		queue->len = 0;

	} else {
		queue->front = queue->front->next; // front of the queue points to the element that comes right after it
		queue->front->prev = NULL;
		temp->next = NULL; // delinks the front of the queue as a stand alone node
		queue->len--; // decrements size of queue
	}
	
	free(temp); // Frees the space held by temp (the queues previous front)
	return 0;
}

/*
	Time Complexity: O(n)
*/
int queue_delete(queue_t queue, void *data)
{
	/* 
		flag: is used to check if the data to be deleted was found in the queue and properly deleted . flag = 0: element not found, flag: 1 element found (first occurence)

		i: is used to track the index of element in the queue while traversing through it. It is initialized to 1 because the traversal logic occurs only when there is more than 1 elements in the queue. 
	*/
	int flag = 0, i = 1;

	if(!data || !queue )
		return -1;

	if(queue->len == 0) {
		return -1;
	}

	struct Node* temp = queue->front;

	/* Checks if the queue has only one element (and so next would point to NULL) and if the data matches the data to be deleted, in which case the queue becomes an empty queue after the node is deleted. */
	if((temp->next == NULL) && (temp->data == data)){
		queue->rear = NULL;
		queue->front = NULL;
		queue->len--;
		free(temp);
		return 0; // return once the first occurance of data is found and deleted
	}

	while(temp != NULL){
		if (temp->data == data)
		{
			flag = 1; // first occurance of data to be deleted is found

			// temporary variables to hold the current node's next and prev pointers
			struct Node *P = temp->prev;
			struct Node *N = temp->next;

			if(i == 1) {
				/* if the queue finds the first occurence of the data at the 
				second position, ie at i == 1, only the front of the queue 
				needs to be manipulated. Then the front of the queue points to the element after it */
				queue->front = N;
				N->prev = NULL;
			} 
			else if(i == queue->len) {
				/* If the element is found at the last position, then the pointer to the rear of the queue points to the element right before it.*/
				queue->rear = P;
				P->next = NULL;
			} else {
				/* In all other cases the pointor to temp is erased by making the element before it point to the element after temp and vice versa using the next and prev pointers in a doubly linked list node. */
				N->prev = P;
				P->next = N;
			}
			// freeing the memory space occupied by temp node
			free(temp);
			queue->len--;
			break;
		}
		// incrementing the counter variable (i) and traversing to the next element in the queue
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

	/* keep an extra temporary pointer temp1 so as to not lose the pointer if a node is deleted*/
	struct Node* temp;
	temp = queue->front;
	struct Node* temp1 = NULL;

	while(temp != NULL){
		temp1 = temp->next;
		func(queue, temp->data);
		temp = temp1;
	}
	return 0;
}


int queue_length(queue_t queue)
{
	/* returns -1 if the pointer to queue doesnt exist, else returns length of queue. */
	if(!queue) {
		return -1;
	}
	// int length = queue->len;
	return queue->len;
}
