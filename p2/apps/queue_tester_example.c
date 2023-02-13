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
	fprintf(stderr, "*** TEST create ***\n");
	queue_t q = queue_create();

	TEST_ASSERT(q != NULL);
	queue_destroy(q);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);

	queue_destroy(q);
}

/* 3 enqueues followed by 3 dequeues */
void enqueue_3_dequeue_3() {

}

/* one enqueue -> one dequeue -> one enqueue -> delete*/
void enqueue_dequeue_enqueue_delete() {

}

/* dequeue when no element : should return -1*/
void enqueue_dequeue_dequeue() {

}

/* delete when no element : should return -1*/
void enqueue_delete_delete() {

}

/* queue is null: try enqueue*/
void null_enqueue() {

}
/* queue is null: try dequeue*/
void null_dequeue() {

}
/* queue is null: try delete*/
void null_delete() {

}

/* tries to create queue when queue is not empty*/
void enqueue_create() {

}

int main(void)
{
	test_create();
	test_queue_simple();

	return 0;
}
