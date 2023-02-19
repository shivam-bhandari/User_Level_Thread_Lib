# Target library
lib := libuthread.a
CFLAGS := -Wall -Werror -Wextra
CC := gcc

# all: $(lib) queue project2

all:  $(lib) 

# project2: $(lib)
# 	gcc -o $@ $< -lm

# $(lib): queue.o
# 	ar rcs $@ $^
$(lib): queue.o context.o uthread.o preempt.o sem.o
	ar rcs $@ $^ 

# hello: uthread.o
# 	$(CC) -o $@ $^ -g

%.o: %.c 
	$(CC) -c $< -o $@

# context.o: context.c private.h
# 	$(CC) -c context.c -o context.o

# uthread.o: uthread.c uthread.h context.c queue.h private.h
# 	$(CC)  -c -r -g uthread.c -o uthread.o


# queue: queue.o
# 	$(CC) -o $@ $^ -g
	
# queue.o: queue.c queue.h
# 	$(CC) -c $(CFLAGS)  -r -g $<

clean: 
	rm -f $(lib) *.o