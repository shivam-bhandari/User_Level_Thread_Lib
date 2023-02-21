#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>

void thread2(void* arg)
{
    (void)arg;
	printf("Entered thread 2\n");
	exit(0);
}

void thread1(void* arg)
{
    (void)arg;
    printf("Entered thread 1\n");
	uthread_create(thread2,arg);
	printf("Created thread 2\n");

	// The only way for thread 2 to be entered is if an interrupt
	// causes it to switch from thread1 to thread2.
	while (1) {}
}

int main(void)
{
	uthread_run(true,thread1, NULL);

	return 0;
}