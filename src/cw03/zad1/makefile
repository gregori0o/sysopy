cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -o main

clean:
	rm -f main

test: compile
	echo "Test działania zadanie 1"
	echo "10 procesów"
	./main 10
	echo "50 procesów"
	./main 50
	echo "100 procesów"
	./main 100
	make clean
