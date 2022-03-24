cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -lpthread -o main

clean:
	rm -f main

test: compile
	./main
	make clean
