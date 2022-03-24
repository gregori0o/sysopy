cc = gcc -Wall -g
size = 10

all: compile

compile:
	$(cc) producent.c -o producent
	$(cc) customer.c -o customer

clean:
	rm -f customer producent

test: compile
	echo "Test działania zadanie 3a"
	echo "Wynik oczekiwany: "
	cat f1.txt
	cat f2.txt
	cat f3.txt
	cat f4.txt
	cat f5.txt
	cat f6.txt
	cat f7.txt
	cat f8.txt
	touch res.txt
	mkfifo potok
	./producent potok 1 f1.txt $(size) &
	./producent potok 5 f5.txt $(size) &
	./producent potok 6 f6.txt $(size) &
	./producent potok 2 f2.txt $(size) &
	./producent potok 4 f4.txt $(size) &
	./producent potok 8 f8.txt $(size) &
	./producent potok 7 f7.txt $(size) &
	./producent potok 3 f3.txt $(size) &
	./customer potok res.txt $(size)
	echo "Koniec działania programu, otrzymano plik: "
	cat res.txt
	rm potok
	rm res.txt
	make clean