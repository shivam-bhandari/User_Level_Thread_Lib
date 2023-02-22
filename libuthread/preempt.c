#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */

#define HZ 100

// global variable to keep track of if preempt is required or not
int preempt_flag = 0;

// setting up signal
struct sigaction new_act, old_act;

// setting up timer
struct itimerval new_tim, old_tim;

// used to store the blocked and old signals
sigset_t block;
sigset_t old_set;

void signal_handler(int signum)
{
	(void)signum;
	uthread_yield();
}

// blocks preemption
void preempt_disable(void)
{
	if (preempt_flag == 0)
	{
		return;
	}
	sigprocmask(SIG_BLOCK, &block, NULL);
}

// unblocks preemption
void preempt_enable(void)
{
	if (preempt_flag == 0)
	{
		return;
	}
	sigprocmask(SIG_UNBLOCK, &block, NULL);
}

/* Refered to resources listed in the ReadMe to set up signal handler */
void preempt_start(bool preempt)
{
	if (preempt == false)
	{
		preempt_flag = 0;
		return;
	}

	/* Signal handler set up; reset the flags and mask; and set the SIGVTALRM; sigprocmask used to save the old set signals */
	sigemptyset(&new_act.sa_mask);
	new_act.sa_flags = 0;
	sigaddset(&new_act.sa_mask, SIGVTALRM);
	sigprocmask(SIG_SETMASK, NULL, &old_set);
	new_act.sa_handler = signal_handler;
	sigaction(SIGVTALRM, &new_act, &old_act);

	/* sets a virtual timer that waits for 10,000 seconds (1000000/HZ) */
	new_tim.it_interval.tv_sec = 0;
	new_tim.it_interval.tv_usec = 1000000 / HZ;
	new_tim.it_value.tv_sec = 0;
	new_tim.it_value.tv_usec = 1000000 / HZ;
	setitimer(ITIMER_VIRTUAL, &new_tim, &old_tim);
}

void preempt_stop(void)
{
	if (preempt_flag == 0)
	{
		return;
	}
	/* Resets the virtual timer, signal to its original state*/
	setitimer(ITIMER_VIRTUAL, &old_tim, NULL);
	sigprocmask(SIG_SETMASK, &old_set, NULL);
	sigaction(SIGVTALRM, &old_act, NULL);
}
