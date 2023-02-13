# Target library
lib := libuthread.a
CFLAGS := -Wall -Werror -Wextra
CC := gcc

# all: $(lib) queue project2

all:  libuthread.a 

# project2: $(lib)
# 	gcc -o $@ $< -lm

# $(lib): queue.o
# 	ar rcs $@ $^
libuthread.a: queue.o 
	ar rcs $@ $^ 

# hello: uthread.o
# 	$(CC) -o $@ $^ -g

# uthread.o: uthread.c uthread.h private.h context.c queue.h 
# 	$(CC)  -c -r -g $<


# queue: queue.o
# 	$(CC) -o $@ $^ -g
	
queue.o: queue.c queue.h
	$(CC) -c $(CFLAGS)  -r -g $<

clean: 
	rm -f %.o $(lib) queue  