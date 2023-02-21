#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>

void thread2(void* arg)
{
    (void)arg;
	printf("Thread 2\n");
	exit(0);
}

void thread1(void* arg)
{
    (void)arg;
    printf("Thread 1\n");
	uthread_create(thread2,arg);
	while (1) {}
}

int main(void)
{
	uthread_run(true,thread1, NULL);

	return 0;
}