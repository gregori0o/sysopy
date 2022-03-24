cc = gcc -Wall -g

all: compile

compile:
	$(cc) server.c -o server -lrt
	$(cc) client.c -o client -lrt

clean:
	rm -f client server

test: compile
	echo "Test działania zadanie 2"

	make clean
