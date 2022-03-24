cc = gcc -Wall -g

all: compile

compile:
	$(cc) zad3_lib.c -o main_lib -lm
	$(cc) zad3_sys.c -o main_sys -lm

clean:
	rm -f main_sys main_lib

test: compile
	echo "Raport zadania 1" > pomiar_zad_3.txt
	make small
	make big
	make clean

small:
	echo "Pomiar dla krótkich plików (ok. 100 lini)" >> pomiar_zad_3.txt
	./main_lib dane_small.txt
	cat report.txt >> pomiar_zad_3.txt
	rm report.txt
	./main_sys dane_small.txt
	cat report.txt >> pomiar_zad_3.txt
	rm report.txt

big:
	echo "Pomiar dla długich plików (ok. 1 000 000 lini)" >> pomiar_zad_3.txt
	./main_lib dane_big.txt
	cat report.txt >> pomiar_zad_3.txt
	rm report.txt
	./main_sys dane_big.txt
	cat report.txt >> pomiar_zad_3.txt
	rm report.txt