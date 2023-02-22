#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)                 \
	do                                      \
	{                                       \
		printf("ASSERT: " #assert " ... "); \
		if (assert)                         \
		{                                   \
			printf("PASS\n");               \
		}                                   \
		else                                \
		{                                   \
			printf("FAIL\n");               \
			exit(1);                        \
		}                                   \
	} while (0)

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
	fprintf(stderr, "\n*** TEST queue_simple ***\n");

	int data = 3, *ptr;
	queue_t q;

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void **)&ptr);

	TEST_ASSERT(ptr == &data);

	queue_destroy(q);
}

/* 3 enqueues followed by 3 dequeues: normal functionality check */
void enqueue_3_dequeue_3()
{
	fprintf(stderr, "\n*** TEST 3 enquques followed by 3 dequeues ***\n");

	queue_t q = queue_create();
	int data1 = 10, data2 = 20, data3 = 30, *ptr;

	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);

	queue_dequeue(q, (void **)&ptr);
	queue_dequeue(q, (void **)&ptr);
	queue_dequeue(q, (void **)&ptr);

	TEST_ASSERT(*ptr == data3);

	queue_destroy(q);
}

/* 3 tests:
1. 3 enqueues followed by delete
2. delete once more (element does not exist in the queue)
3. dequeue after a failed delete */
void enqueue_3_delete()
{
	fprintf(stderr, "\n*** TEST 3 enqueues and one delete ***\n");

	queue_t q;
	q = queue_create();

	int data1 = 10, data2 = 20, data3 = 10, *ptr;

	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);

	queue_delete(q, &data3);

	TEST_ASSERT(queue_length(q) == 2);

	fprintf(stderr, "\n*** TEST delete one more time but (element does not exist) ***\n");

	int data4 = 50;

	TEST_ASSERT(queue_delete(q, &data4) == -1);

	fprintf(stderr, "\n*** TEST dequeue after failed delete ***\n");

	queue_dequeue(q, (void **)&ptr);

	TEST_ASSERT(*ptr == data1);

	queue_dequeue(q, (void **)&ptr);
	queue_destroy(q);
}

/* dequeue when no element : should return -1*/
void enqueue_dequeue_2()
{
	fprintf(stderr, "\n*** TEST enqueue and 2 dequeues ***\n");

	queue_t q = queue_create();
	int data = 10, *ptr;

	queue_enqueue(q, &data);
	queue_dequeue(q, (void **)&ptr);

	TEST_ASSERT(queue_dequeue(q, (void **)&ptr) == -1);

	queue_destroy(q);
}

/* delete when no element in queue*/
void enqueue_delete_2()
{
	fprintf(stderr, "\n*** TEST enqueue and 2 deletes ***\n");

	queue_t q = queue_create();
	int data = 10;

	queue_enqueue(q, &data);
	queue_delete(q, &data);

	TEST_ASSERT(queue_delete(q, &data) == -1);

	queue_destroy(q);
}

/* try enqueue when queue is null*/
void null_enqueue()
{
	fprintf(stderr, "\n*** TEST enqueue when queue is NULL ***\n");

	queue_t q = NULL;
	int data = 10;

	TEST_ASSERT(queue_enqueue(q, &data) == -1);

	queue_destroy(q);
}

/*  try dequeue when queue is null*/
void null_dequeue()
{
	fprintf(stderr, "\n*** TEST dequeue when queue is NULL ***\n");

	queue_t q = NULL;
	int *ptr;
	TEST_ASSERT(queue_dequeue(q, (void **)&ptr) == -1);

	queue_destroy(q);
}

/* try delete when queue is null*/
void null_delete()
{
	fprintf(stderr, "\n*** TEST delete when queue is NULL ***\n");

	queue_t q = NULL;
	int data = 10;
	TEST_ASSERT(queue_delete(q, &data) == -1);

	queue_destroy(q);
}

/* test to check if program returns -1 if NULL data is passed as second argument to the enqueue, dequeue and delete functions*/
void data_null()
{
	queue_t q = queue_create();

	fprintf(stderr, "\n*** TEST enqueue NULL to the queue ***\n");
	TEST_ASSERT(queue_enqueue(q, NULL) == -1);

	fprintf(stderr, "\n*** TEST dequeue NULL to the queue ***\n");
	TEST_ASSERT(queue_dequeue(q, NULL) == -1);

	fprintf(stderr, "\n*** TEST delete NULL to the queue ***\n");
	TEST_ASSERT(queue_delete(q, NULL) == -1);

	queue_destroy(q);
}

/* tester function for queue_iterate*/
static void iterator_inc(queue_t q, void *data)
{
	int *a = (int *)data;
	if (*a == 42)
		queue_delete(q, data);
	else
		*a += 1;
}

/* tests queue iterator */
void test_queue_iterate()
{
	fprintf(stderr, "\n*** TEST queue_iterator functionality ***\n");

	queue_t q = queue_create();
	int data1 = 10, data2 = 20, data3 = 30, *ptr;

	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);

	queue_iterate(q, iterator_inc);

	int data = 40;
	TEST_ASSERT(queue_delete(q, &data) == -1 && queue_length(q) == 3);

	queue_dequeue(q, (void **)&ptr);
	queue_dequeue(q, (void **)&ptr);
	queue_destroy(q);
}

/* tests if delete works in the queue_iterator function */
void test_iterate_delete()
{
	fprintf(stderr, "\n*** TEST queue_iterator that deletes when data = 42 ***\n");

	queue_t q = queue_create();
	int data1 = 10, data2 = 42, data3 = 30, *ptr;

	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);

	queue_iterate(q, iterator_inc);

	data3 = 31;
	TEST_ASSERT(queue_delete(q, &data3) == 0);

	queue_dequeue(q, (void **)&ptr);
	queue_destroy(q);
}

int main(void)
{
	test_create();
	test_queue_simple();
	enqueue_3_dequeue_3();
	enqueue_3_delete();
	enqueue_dequeue_2();
	enqueue_delete_2();
	null_enqueue();
	null_dequeue();
	null_delete();
	data_null();
	test_queue_iterate();
	test_iterate_delete();

	return 0;
}
