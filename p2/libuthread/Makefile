# Target library
# lib := libuthread.a

# all: $(lib) queue project2

all: queue

# project2: $(lib)
# 	gcc -o $@ $< -lm

# $(lib): queue.o
# 	ar rcs $@ $^

queue: queue.c queue.h
	gcc  -o $@ $< -g
	
# queue.o: queue.c queue.h
# 	gcc -Wall -Werror -Wextra -o $@ $<

clean: 
	rm -f queue