cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -o main

clean:
	rm -f main

test: compile
	echo "Test działania zadanie 1"
	./main cm.txt skladnik1 \| skladnik9
	./main cm.txt skladnik11
	./main cm.txt skladnik11 \| skladnik4
	make clean