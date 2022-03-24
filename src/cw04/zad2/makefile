cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -o main

clean:
	rm -f main

test: compile
	echo "Test działania zadanie 2" > raport2.txt
	./main siginfo >> raport2.txt
	./main nodefer >> raport2.txt
	./main without >> raport2.txt
	./main resethand 1>> raport2.txt 2>> raport2.txt
	make clean