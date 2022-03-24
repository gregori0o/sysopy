cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -o main
	$(cc) cook.c -o cook
	$(cc) supplier.c -o supplier

clean:
	rm -f supplier cook main

small_test: compile
	echo "Test działania zadanie 1"
	echo "3 kucharzy i 4 dostawców"
	./main 3 4
	make clean

big_test: compile
	echo "Test działania zadanie 1"
	echo "10 kucharzy i 8 dostawców"
	./main 10 8
	make clean

med_test: compile
	echo "Test działania zadanie 1"
	echo "10 kucharzy i 1 dostawca"
	./main 10 1
	make clean
