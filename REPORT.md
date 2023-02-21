# ECS150P2

## PHASE 1: queue.c
Phase 1 involved us building queue.c and queue_tester_example.c. For
implementing the queue, we first decided to use a Linked List. This is because
it would help us accomplish O(1) time for all our operations except delete and
iterate. However, we got stuck in the delete aspect of our code. This is 
because we would lose the pointer if a delete operation were to take place. 
Hence we changed our implementation to a Double Linked List. This helped us 
with the delete operation. An indepth explanation of our code is in the queue.c
file, however, this report will explain some of the functions and the tester 
briefly. We also ran into many errors involving the void pointer which we were 
finally able to solve with the help of the TAs.

### queue_enqueue()
Adds an element to the end of the queue. General case involved manipulating the
rear pointer to add a new node and reassigning the rear node to the newnode. In
the case of an empty queue, we create a new node and assign front and rear
pointers to the only node.

### queue_dequeue()
Deletes an element from the beginning of the queue. General case involved
manipulating the front pointer to remove the front node and reassigning the
front node to the next of current front node. In the case of a single element
queue, we just set the front and rear to `NULL`.

### queue_iterate()
Iterates through the function with O(n) time complexity. Does the operation on
each node in the queue.

### queue_delete()
A total of 4 cases. (i) If it's the only element - set rear and front to NULL If
not then it runs a loop - (ii) If it finds it (second element) in the first time
in the loop - set the front of the queue to this node, (iii) If the last element
is found - tthe rear of the queue points to the element previous to this one,
(iv) any other case the element previous to the element found, will point to the
element after the element found and vice-versa.

### queue_tester()
The `queue_tester()` has about 15 tests that checks both the normal
functionality of each of the functions and any edge cases. It also tries to do a
valid command after doing an invlid command. For example, we have a test case
that tries to delete a data element that does not exist in the queue (which
fails successfully) and then correctly does the following command, (in our case
a dequeue)!

## PHASE 2: uthread.c
This was the toughest part of our code. We decided to use 2 queues, a dead queue
and a regular queue, storing the states of each of the threads. For uthread_run,
we save the current running thread to idle thread, and keep yielding to the next
threads till only the idle thread is left. In doing so, we ensure every thread
gets a share of CPU time. uthread_create has a simple task, initialize the
context and stack of the threads and enqueue them. uthread_exit stores the
thread into the dead_queue. uthread_yield required the most work. We first save
the current thread and enqueue it to queue. We then dequeue the next thread and
switch to that thread using a context switch. We tested this phase using the
given testers, uthread_hello and uthread_yield and they passed the tests
successfully.

## PHASE 3: sem.c
We like to think of semaphores like restaurants, and threads as people waiting
in line to get tables at these fancy restaurants. Our `semaphore` struct was
essentially the count and the blocked list itself. The functions were fairly
simple to implement. `sem_create()` initializes the `blocked_list` and `semaphore_count`,
while destroy deallocates memory associated with the sem. `sem_up()` 
increases the count and provides resources to the blocked threads, while `sem_down()`
put threads to wait in the blocked list and decremented resources. We tested
these using `sem_simple()`, `sem_count()`, `sem_prime()` and `sem_buffer()` and they passed the
test cases successfully.

## PHASE 4: preempt.c
We set up a signal handler `signal_handler()` to yield to the next thread. To
set the signal we reset the flags and mask, set the `SIGVTALRM`, and preserve
the old sate of signals. Also set up a virtual timer by setting the
`it_interval` and `it_value` and using `setitimer`. To stop the preemption, we
reset the timer and signal state to its previous states in `preemt_stop`.
`preempt_enable()` and 'preemebt_disable()` simply unblocks and blocks the
preemption using `sigprocmask()`.

### tester function: test_preempt.c
We use `test_preempt.c` to test our preemption. Why our test function works? If
preemption was incorrect, the program would never yield to the `thread2`, thus
never exiting the program and staying blank after printing "Thread 1". However, 
we see "Thread 2" being printed, and the program exiting, hence proving our 
preemption works!

## Resources
All the code in the project was written by us. However, we did make use of the
resources provided to us by professor's lecture and discussion slides. We also
made good use of the TA Office Hours.

### Cited Links
1. [Queue
   implementation](https://www.geeksforgeeks.org/implementation-deque-using-doubly-linked-list/)
2. [Preemption](https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/sigactn.html)
3. [Signal handler
   1](https://pubs.opengroup.org/onlinepubs/007904875/functions/sigaction.html)
   [Signal Handler
   2](https://jameshfisher.com/2017/01/13/c-sigaction/#:~:text=sigactionsig%2C%20act%2C%20oactthe%20given%20signal%20is%20received)
 