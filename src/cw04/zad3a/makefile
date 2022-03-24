cc = gcc -Wall -g

all: compile

compile:
	$(cc) catcher.c -o catcher
	$(cc) sender.c -o sender

clean:
	rm -f catcher sender

test: compile
	echo "Test działania zadanie 3a" > raport3a.txt
	make test_KILL
	make test_SIGQUEUE
	make test_SIGRT	
	make clean

test_KILL:
	echo "Test dla polecenia KILL" >> raport3a.txt
	echo "100 sygnałów" >> raport3a.txt
	./catcher >> raport3a.txt &
	./sender `pidof -s catcher` 100 "KILL" >> raport3a.txt
	echo "1000 sygnałów" >> raport3a.txt
	./catcher >> raport3a.txt &
	./sender `pidof -s catcher` 1000 KILL >> raport3a.txt

test_SIGQUEUE:
	echo "Test dla polecenia SIGQUEUE" >> raport3a.txt
	echo "100 sygnałów" >> raport3a.txt
	./catcher SIGQUEUE >> raport3a.txt &
	./sender `pidof -s catcher` 100 SIGQUEUE >> raport3a.txt
	echo "1000 sygnałów" >> raport3a.txt
	./catcher SIGQUEUE >> raport3a.txt &
	./sender `pidof -s catcher` 1000 SIGQUEUE >> raport3a.txt

test_SIGRT:
	echo "Test dla polecenia SIGRT" >> raport3a.txt
	echo "100 sygnałów" >> raport3a.txt
	./catcher SIGRT >> raport3a.txt &
	./sender `pidof -s catcher` 100 SIGRT >> raport3a.txt
	echo "1000 sygnałów" >> raport3a.txt
	./catcher SIGRT >> raport3a.txt &
	./sender `pidof -s catcher` 1000 SIGRT >> raport3a.txt