# ECS150P2

## PHASE 1
Phase 1 involved us building queue.c and queue_tester_example.c. For 
implementing the queue, we first decided to use a Linked List. This is because it would help us accomplish O(1) time for all our operations except delete and iterate. However, we got stuck in the delete aspect of our code. This is because we would lose the pointer if a delete operation were to take place. Hence we changed our implementation to a Double Linked List. This helped us with the delete operation. An indepth explanation of our code is in the queue.c file, however, this report will explain some of the functions and the tester briefly.

### queue_enqueue()
Adds an element to the end of the queue. General case involved manipulating the rear pointer to add a new node and reassigning the rear node to the newnode. In the case of an empty queue, we create a new node and assign front and rear pointers to the only node.
## PHASE 2

## PHASE 3

## PHASE 4