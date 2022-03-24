cc = gcc -Wall -g

all: compile

compile:
	$(cc) zad1_lib.c -o main_lib
	$(cc) zad1_sys.c -o main_sys

clean:
	rm -f main_sys main_lib

test: compile
	echo "Raport zadania 1" > pomiar_zad_1.txt
	make small
	make big
	make clean

small:
	echo "Pomiar dla krótkich plików (ok. 100 lini)" >> pomiar_zad_1.txt
	./main_lib small1.txt small2.txt
	cat report.txt >> pomiar_zad_1.txt
	rm report.txt
	./main_sys small1.txt small2.txt
	cat report.txt >> pomiar_zad_1.txt
	rm report.txt

big:
	echo "Pomiar dla długich plików (ok. 100 000 lini)" >> pomiar_zad_1.txt
	./main_lib big1.txt big2.txt
	cat report.txt >> pomiar_zad_1.txt
	rm report.txt
	./main_sys big1.txt big2.txt
	cat report.txt >> pomiar_zad_1.txt
	rm report.txt