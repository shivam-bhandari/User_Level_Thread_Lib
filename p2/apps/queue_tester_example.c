#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "\n*** TEST create ***\n");
	queue_t q = queue_create();

	TEST_ASSERT(q != NULL);
	queue_destroy(q);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "\n*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);

	queue_destroy(q);
}

/* 3 enqueues followed by 3 dequeues */
void enqueue_3_dequeue_3() {
	queue_t q = queue_create();

	fprintf(stderr, "\n*** TEST 3 enquques followed by 3 dequeues ***\n");

	int data1 = 10, data2 = 20, data3 = 30, *ptr;
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);

	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr);

	// printf("returned val: %d\n", *ptr);

	TEST_ASSERT(*ptr == data3);

	queue_destroy(q);
}

/* one enqueue -> one dequeue -> one enqueue -> delete*/

void enqueue_3_delete() {
	queue_t q;
	q = queue_create();

	fprintf(stderr, "\n*** TEST 3 enqueues and one delete ***\n");

	int data1 = 10, data2 = 20, data3 = 10, *ptr;
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);

	queue_delete(q, &data3);
	
	// printf("second element val: %d\n", queue_length(q));
	TEST_ASSERT(queue_length(q) == 2);

	fprintf(stderr, "\n*** TEST delete one more time but (element does not exist) ***\n");
	int data4 = 50;
	TEST_ASSERT(queue_delete(q, &data4) == -1);

	fprintf(stderr, "\n*** TEST dequeue after failed delete ***\n");

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(*ptr == data1);

	queue_dequeue(q, (void**)&ptr);
	// printf("now deallocating\n");
	queue_destroy(q);
}

/* dequeue when no element : should return -1*/
void enqueue_dequeue_2() {
	queue_t q = queue_create();

	fprintf(stderr, "\n*** TEST enqueue and 2 dequeues ***\n");

	int data = 10, *ptr;
	queue_enqueue(q, &data);

	queue_dequeue(q, (void**)&ptr);

	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);


	queue_destroy(q);
}

/* delete when no element : should return -1*/
void enqueue_delete_2() {
	queue_t q = queue_create();

	fprintf(stderr, "\n*** TEST enqueue and 2 deletes ***\n");

	int data = 10;

	queue_enqueue(q, &data);
	/* TODO: DELETE DOESNT WORK CORREVTLY IF QUEUE IS EMPTY */
	queue_delete(q, &data);
	TEST_ASSERT(queue_delete(q, &data) == -1);

	queue_destroy(q);
}

/* queue is null: try enqueue*/
void null_enqueue() {
	fprintf(stderr, "\n*** TEST enqueue when queue is NULL ***\n");

	queue_t q = NULL;
	int data = 10;
	TEST_ASSERT(queue_enqueue(q, &data) == -1);

	queue_destroy(q);
}
/* queue is null: try dequeue*/
void null_dequeue() {
	fprintf(stderr, "\n*** TEST dequeue when queue is NULL ***\n");

	queue_t q = NULL;
	int  *ptr;
	TEST_ASSERT(queue_enqueue(q, (void**)&ptr) == -1);

	queue_destroy(q);
}
/* queue is null: try delete*/
void null_delete() {
	fprintf(stderr, "\n*** TEST delete when queue is NULL ***\n");

	queue_t q = NULL;
	int data = 10;
	TEST_ASSERT(queue_delete(q, &data) == -1);

	queue_destroy(q);
}

/* tries to create queue when queue is not empty*/
void enqueue_create() {
	int data = 10;
	queue_t q;
	q = queue_create();

	fprintf(stderr, "\n*** TEST 3create queue after enqueing  ***\n");

	queue_enqueue(q, &data);
	data = 20;
	queue_enqueue(q, &data);

	TEST_ASSERT(queue_create() == NULL);

	queue_destroy(q);
}

int main(void)
{
	test_create();
	test_queue_simple();
	enqueue_3_dequeue_3();
	/* 
	1. 3 enqueues followed by one delete
	2. delete once more but element does not exist
	*/
	enqueue_3_delete(); 

	
	enqueue_dequeue_2();

	
	enqueue_delete_2();

	null_enqueue();
	null_dequeue();
	null_delete();

	// fix this: 
	// enqueue_create();

	return 0;
}
