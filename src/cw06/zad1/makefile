cc = gcc -Wall -g

all: compile

compile:
	$(cc) server.c -o server
	$(cc) client.c -o client

clean:
	rm -f client server

test: compile
	echo "Test działania zadanie 1"

	make clean
