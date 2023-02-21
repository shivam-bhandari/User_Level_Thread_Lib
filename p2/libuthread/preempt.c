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
struct sigaction new_act, old_act;
// setting up timer
struct itimerval new_tim, old_tim;
// used to store the blocked and old signals
sigset_t block; 
sigset_t old_set; 

void signal_handler(int signum)
{
	(void)signum;
	// printf("Beep Beep\n");
	uthread_yield();
}

void preempt_disable(void)
{
	if (preempt_flag == 0){
		return;
	}
	sigprocmask(SIG_BLOCK, &block, NULL);
}

void preempt_enable(void)
{
	if (preempt_flag == 0){
		return;
	}
	sigprocmask(SIG_UNBLOCK, &block, NULL);
}

	// install a sighandler
	// https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/sigactn.html
	// https://pubs.opengroup.org/onlinepubs/007904875/functions/sigaction.html
	// https://jameshfisher.com/2017/01/13/c-sigaction/#:~:text=sigaction(sig%2C%20act%2C%20oact,the%20given%20signal%20is%20received.
	// The above resources were used for this phase
void preempt_start(bool preempt)
{
	if (preempt == false){
		preempt_flag = 0;
		return;
	}
	
	sigemptyset(&new_act.sa_mask);
	new_act.sa_flags = 0;
	sigaddset(&new_act.sa_mask, SIGVTALRM);
	sigprocmask(SIGVTALRM, NULL, &old_set);
	new_act.sa_handler = (__sighandler_t)signal_handler;
	sigaction(SIGVTALRM, &new_act, &old_act);

	new_tim.it_interval.tv_sec = 0;
	new_tim.it_interval.tv_usec = 10000;
	new_tim.it_value.tv_sec = 0;
	new_tim.it_value.tv_usec = 10000;

	setitimer(ITIMER_VIRTUAL, &new_tim, &old_tim);

}

void preempt_stop(void)
{
	if (preempt_flag == 0){
		return;
	}
	setitimer(ITIMER_VIRTUAL, &old_tim, NULL);
	sigprocmask(SIG_SETMASK, &old_set, NULL);
	if(sigaction(SIGVTALRM, &old_act, NULL) < 0){
		perror(":sigaction error:");
	}
}

// save old one
// empty the new one
// sigaddset