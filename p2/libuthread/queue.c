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
}

int queueEmpty(queue_t queue){
	return queue->len == 0;
}

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	printf("in queue create:\n");
	queue_t q = (queue_t)malloc(sizeof(struct queue));
    q->front = NULL;
	q->rear = NULL;
	q->len = 0;
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
	printf("in queue enqueue\n");
	/* TODO Phase 1 */
	if ( !data || !queue) 
		return -1;

	// create new node
	struct Node* newNode = malloc(sizeof(struct Node));

	// check for any malloc error
	if(!newNode) 
	{
		printf("malloc error\n");
		return -1;
	}
	
	// assign value 
	newNode->data = data;
	//printf("data: %p\n",newNode->data);

	if (queueEmpty(queue)) {
		queue->rear = newNode;
		queue->front = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;
	}
	else {
		// printf("else:\n");
		queue->rear->next = newNode;
		queue->rear->prev = queue->rear;
		queue->rear = queue->rear->next;
		queue->rear->next = NULL;
	} 

	// printf("print front of queue\n");
	// printf("front: %p\n", queue->front->data);
	
	//queue->len++;
	print_queue(queue);
	return 0;
}

// @TRISHNA IMPLEMENT THIS

int queue_dequeue(queue_t queue, void **data)
{
	//printf("old queue len : %d\n",queue_length(queue));
	printf("in queue dequeue\n");
	if(queueEmpty(queue) || !queue || data == NULL) 
	{
		printf("queue or data NULL; return -1\n");
		return -1;
	}

	*data = queue->front->data;

	// if(!queueEmpty(queue)) {  
	//	 *data = queue->front->data;

	// 	//printf("queue not empty\n");

	// 	struct Node* tmp = queue->front;
	// 	// case 1: front and rear of q are not the same nodes
	// 	if(queue->len > 1) {

	// 		//printf("quque len > 1\n");

	// 		queue->front = queue->front->next;
	// 		queue->front->prev = NULL;
			
	// 		// queue->front->next = tmp->next->next;
			
	// 	} else {

	// 		//printf("queue len = 1\n");
			
	// 		queue->front = NULL;
	// 		queue->rear = NULL;

	// 	}
	// 	// *data = tmp->data; // pointer allocation maybe
	// 	free(tmp);


	struct Node* temp = queue->front;

	queue->front = queue->front->next;
	
	if(queue->front == NULL)
		queue->rear = NULL;
	else
		queue->front->prev = NULL;

	free(temp);
	// queue->len--;
	
	print_queue(queue);

	printf("new queue len : %d\n", queue->len);

	return 0;
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	int flag = 0; // flag to check if data is found

	if(!data || !queue)
		return -1;

	struct Node* temp = queue->front;

	// if queue has only one element

	if((temp->next == NULL) && (temp->data == data)){
		flag = 1;
		queue->rear = NULL;
		queue->front = NULL;
		free(temp);
		return 0;
	}

	while(temp!=NULL){

		if (temp->data == data)
		{
			flag = 1;
			struct Node *T = temp;
			temp->prev->next = T->next;
			temp->next->prev = T->prev;

			free(T);
			break;
		}
		temp = temp->next;
	}
		
	queue->len--;
	if(flag == 0)
		return flag;
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
	//rintf("doing queue_enqueue:\n");
	queue_enqueue(q, (void*)10);

	// printf("doing second enqueque now\n");

	queue_enqueue(q, (void*)20);
	printf("queue length: %d\n", queue_length(q));

	queue_dequeue(q, (void**)temp);
	// // printf("queue length: %d\n", q->len);
	// printf("\ntemp int dequed: %d\n\n", *temp);
	// printf("Everything good till here \n");
	// printf("queue length: %d\n", queue_length(q));

	// printf("Done w second enqueue...\n");
	// int* 
	//printf("doing queue_front:\n");
	
	queue_enqueue(q, (void*)30);
	printf("queue length: %d\n", queue_length(q));
	print_queue(q);
	// queue_dequeue(q, (void**)temp);
	// queue_destroy()
	

	
	return 0;
}