cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -o main

clean:
	rm -f main

test: compile
	echo "Test działania zadanie 3, poszukiwania w katalogu domowym"
	echo "Tekst ' ', głębokość - 2"
	./main ~ " " 2
	echo "Tekst 'while', głębokość - 4"
	./main ~ "while" 4
	echo "Tekst 'for', głębokość - 5"
	./main ~ "for" 5
	echo "Tekst 'abracadabra', głębokość - 100"
	./main ~ "abracadabra" 100
	make clean
