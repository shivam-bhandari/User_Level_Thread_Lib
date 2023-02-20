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
int preempt_flag = 1;
// setting up signal
struct sigaction new_act;
struct sigaction old_act;
// setting up timer
struct itimerval new_tim;
struct itimerval old_tim;
// used to store the blocked and old signals
sigset_t block; 
sigset_t old_set; 

void handler(int signum)
{
	if (signum == SIGVTALRM)
		uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	if (preempt_flag == 0){
		return;
	}
	sigprocmask(SIG_BLOCK, &block, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	if (preempt_flag == 0){
		return;
	}
	sigprocmask(SIG_UNBLOCK, &block, NULL);
}

void preempt_start(bool preempt)
{
	if (preempt == 0){
		preempt_flag = 0;
		return;
	}
	/* TODO Phase 4 */
	// install a sighandler
	// https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/sigactn.html
	// https://pubs.opengroup.org/onlinepubs/007904875/functions/sigaction.html
	// https://jameshfisher.com/2017/01/13/c-sigaction/#:~:text=sigaction(sig%2C%20act%2C%20oact,the%20given%20signal%20is%20received.
	// The above resources were used for this phase

	new_act.sa_handler = handler;
	sigemptyset(&new_act.sa_mask); // empty the set and set the flags to 0
	new_act.sa_flags = 0;
	// configure a timer
	new_tim.it_interval.tv_sec = 0;
	new_tim.it_value.tv_sec = 0;
	

	sigemptyset(&block);
	sigaddset(&block, SIGVTALRM); // The alarm signal responsible for blocking.

	new_tim.it_interval.tv_usec = (1000000/HZ);
	new_tim.it_value.tv_usec = (1000000/HZ);

	sigprocmask(SIG_SETMASK, NULL, &old_set); // Store main thread signal mask
	setitimer(ITIMER_VIRTUAL, &new_tim, &old_tim);
	sigaction(SIGVTALRM, &new_act, &old_act);  // Signal handler initialization
}

void preempt_stop(void)
{
	if (preempt_flag == 0){
		return;
	}
	setitimer(SIGVTALRM, &old_tim, NULL);
	sigaction(SIGVTALRM, &old_act, NULL);
	sigprocmask(SIG_SETMASK, &old_set, NULL);
	/* TODO Phase 4 */
}

