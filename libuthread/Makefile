# Target library
lib := libuthread.a
CFLAGS := -Wall -Werror -Wextra
CC := gcc


all:  $(lib) 

$(lib): queue.o context.o uthread.o preempt.o sem.o
	ar rcs $@ $^ 

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(lib) *.o