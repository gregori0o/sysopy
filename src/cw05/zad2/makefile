cc = gcc -Wall -g

all: compile

compile:
	$(cc) main.c -o main

clean:
	rm -f main

test: compile
	echo "Test działania zadanie 2"
	./main "~"
	./main "kot" "kotek  kot zjadł mysz"
	./main "pies" "kotek goni psa"
	make clean