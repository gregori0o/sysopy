cc = gcc -Wall -g
size = 30

all: compile

compile:
	$(cc) producent.c -o producent
	$(cc) customer.c -o customer
	$(cc) main.c -o main

clean:
	rm -f customer producent main

test: compile
	echo "Test działania zadanie 3b" > raport.txt
	echo "Oczekujemy uzyskania pliku: " >> raport.txt
	cat f1.txt >> raport.txt
	cat f2.txt >> raport.txt
	cat f3.txt >> raport.txt
	cat f4.txt >> raport.txt
	cat f5.txt >> raport.txt
	cat f6.txt >> raport.txt
	cat f7.txt >> raport.txt
	cat f8.txt >> raport.txt
	make test_in_bash
	make test_in_file
	make clean


test_in_bash:
	echo "Test działania potoku utworzonego w bashu" >> raport.txt
	touch res.txt
	mkfifo potok
	./customer potok res.txt $(size) &
	./producent potok 1 f1.txt $(size) &
	./producent potok 5 f5.txt $(size) &
	./producent potok 6 f6.txt $(size) &
	./producent potok 2 f2.txt $(size) &
	./producent potok 4 f4.txt $(size) &
	./producent potok 8 f8.txt $(size) &
	./producent potok 7 f7.txt $(size) &
	./producent potok 3 f3.txt $(size)
	echo "Koniec działania programu, otrzymano plik: " >> raport.txt
	cat res.txt >> raport.txt
	echo "" >> raport.txt
	rm potok
	rm res.txt

test_in_file:
	echo "Test działania potoku utworzonego w pliku" >> raport.txt
	touch res.txt
	./main
	echo "Koniec działania programu, otrzymano plik: " >> raport.txt
	cat res.txt >> raport.txt
	rm potok
	rm res.txt
