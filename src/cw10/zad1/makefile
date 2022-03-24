cc = gcc -Wall -g

all: compile

compile:
	$(cc) server.c -lpthread -o server
	$(cc) client.c -lpthread -o client

clean:
	rm -f client server
