cc = gcc -Wall -g

all: compile

compile:
	$(cc) zad2_lib.c -o main_lib
	$(cc) zad2_sys.c -o main_sys

clean:
	rm -f main_sys main_lib

test: compile
	echo "Raport zadania 2" > pomiar_zad_2.txt
	make small
	make big
	make clean

small:
	echo "Pomiar dla krótkich plików (ok. 100 lini)" >> pomiar_zad_2.txt
	echo "Dla litery występującej w tekście" >> pomiar_zad_2.txt
	./main_lib e small1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt
	./main_sys e small1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt
	echo "Dla litery nie występującej w tekście" >> pomiar_zad_2.txt
	./main_lib Q small1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt
	./main_sys Q small1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt

big:
	echo "Pomiar dla długich plików (ok. 100 000 lini)" >> pomiar_zad_2.txt
	echo "Dla litery występującej w tekście" >> pomiar_zad_2.txt
	./main_lib e big1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt
	./main_sys e big1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt
	echo "Dla litery nie występującej w tekście" >> pomiar_zad_2.txt
	./main_lib Q big1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt
	./main_sys Q big1.txt
	cat report.txt >> pomiar_zad_2.txt
	rm report.txt