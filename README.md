# ECS150P2

## PHASE 1: queue.c
Phase 1 involved us building queue.c and queue_tester_example.c. For 
implementing the queue, we first decided to use a Linked List. This is because 
it would help us accomplish O(1) time for all our operations except delete and 
iterate. However, we got stuck in the delete aspect of our code. This is because 
we would lose the pointer if a delete operation were to take place. Hence we 
changed our implementation to a Double Linked List. This helped us with the 
delete operation. An indepth explanation of our code is in the queue.c file, 
however, this report will explain some of the functions and the tester briefly.
We also ran into many errors involving the void pointer which we were finally able
to solve with the help of the TAs.

### queue_enqueue()
Adds an element to the end of the queue. General case involved manipulating the
rear pointer to add a new node and reassigning the rear node to the newnode. 
In the case of an empty queue, we create a new node and assign front and rear
pointers to the only node.
### queue_dequeue()
Deletes an element from the beginning of the queue. General case involved 
manipulating the front pointer to remove the front node and reassigning the 
front node to the next of current front node. In the case of a single element 
queue, we just set the front and rear to NULL.
### queue_iterate()
Iterates through the function with O(n) time complexity. Does the operation on 
each node in the queue.
### queue_delete()
A total of 4 cases. (i) If it's the only element - set rear and front to NULL 
If not then it runs a loop - (ii) If it finds it (second element) in the first
time in the loop - set the front of the queue to this node, (iii) If the last 
element is found - tthe rear of the queue points to the element previous to 
this one, (iv) any other case the element previous to the element found, will 
point to the element after the element found and vice-versa.

## PHASE 2: uthread.c
This was probably the part that racked our brains the most.

## PHASE 3: sem.c
We like to think of semaphores like restaurants, and threads as people waiting in 
line to get tables at these fancy restaurants. Our semaphore struct was essentially the count
and the blocked list itself. The functions were fairly simple to implement. sem_create() 
initializes the blocked list and sem count, while destroy deallocates memory associated with 
the sem. sem_up involved increasing the count and providing resources to blocked threads, while
sem_down put threads to wait in the blocked list and decremented resources. 
## PHASE 4: preempt.c
We set up a signal handler `signal_handler()` to yield to the next thread. To 
set the signal we reset the flags and mask, set the `SIGVTALRM`, and 

### tester function: test_preempt.c
We use test_preempt.c to test our preemption.