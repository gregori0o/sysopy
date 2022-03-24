cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -o main

clean:
	rm -f main

test: compile
	echo "Test działania zadanie 1" > raport2.txt
	./main ignore >> raport2.txt
	./main handler >> raport2.txt
	./main mask >> raport2.txt
	./main pending >> raport2.txt
	make clean