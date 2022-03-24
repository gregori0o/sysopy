cc = gcc -Wall -g
pic1 = brain_398.ascii.pgm
pic2 = brain_604.ascii.pgm
pic3 = body3.ascii.pgm
pic4 = dla.ascii.pgm

all: compile

compile:
	$(cc) main.c -lpthread -o main

clean:
	rm -f main res.pgm

test: compile
	./main 2 block $(pic1) res1.pgm
	./main 4 block $(pic2) res2.pgm
	./main 2 numbers $(pic3) res3.pgm
	./main 4 numbers $(pic4) res4.pgm
	make clean

all_test: compile
	echo "Testy działania - każdy dla 4 różnych obrazów" > Times.txt
	#obrazy do testów możemy ustawić w zmiennych pic1, pic2, pic3, pic4
	make numbers_test
	make block_test
	make clean

numbers_test:
	echo "Test metody - numbers" >> Times.txt
	echo "Test dla 1 wątku" >> Times.txt
	./main 1 numbers $(pic1) res.pgm >> Times.txt
	./main 1 numbers $(pic2) res.pgm >> Times.txt
	./main 1 numbers $(pic3) res.pgm >> Times.txt
	./main 1 numbers $(pic4) res.pgm >> Times.txt
	echo "Test dla 2 wątków" >> Times.txt
	./main 2 numbers $(pic1) res.pgm >> Times.txt
	./main 2 numbers $(pic2) res.pgm >> Times.txt
	./main 2 numbers $(pic3) res.pgm >> Times.txt
	./main 2 numbers $(pic4) res.pgm >> Times.txt
	echo "Test dla 4 wątków" >> Times.txt
	./main 4 numbers $(pic1) res.pgm >> Times.txt
	./main 4 numbers $(pic2) res.pgm >> Times.txt
	./main 4 numbers $(pic3) res.pgm >> Times.txt
	./main 4 numbers $(pic4) res.pgm >> Times.txt
	echo "Test dla 8 wątków" >> Times.txt
	./main 8 numbers $(pic1) res.pgm >> Times.txt
	./main 8 numbers $(pic2) res.pgm >> Times.txt
	./main 8 numbers $(pic3) res.pgm >> Times.txt
	./main 8 numbers $(pic4) res.pgm >> Times.txt
	echo "Test dla 16 wątków" >> Times.txt
	./main 16 numbers $(pic1) res.pgm >> Times.txt
	./main 16 numbers $(pic2) res.pgm >> Times.txt
	./main 16 numbers $(pic3) res.pgm >> Times.txt
	./main 16 numbers $(pic4) res.pgm >> Times.txt
	echo "Test dla 64 wątków" >> Times.txt
	./main 64 numbers $(pic1) res.pgm >> Times.txt
	./main 64 numbers $(pic2) res.pgm >> Times.txt
	./main 64 numbers $(pic3) res.pgm >> Times.txt
	./main 64 numbers $(pic4) res.pgm >> Times.txt

block_test:
	echo "Test metody - block" >> Times.txt
	echo "Test dla 1 wątku" >> Times.txt
	./main 1 block $(pic1) res.pgm >> Times.txt
	./main 1 block $(pic2) res.pgm >> Times.txt
	./main 1 block $(pic3) res.pgm >> Times.txt
	./main 1 block $(pic4) res.pgm >> Times.txt
	echo "Test dla 2 wątków" >> Times.txt
	./main 2 block $(pic1) res.pgm >> Times.txt
	./main 2 block $(pic2) res.pgm >> Times.txt
	./main 2 block $(pic3) res.pgm >> Times.txt
	./main 2 block $(pic4) res.pgm >> Times.txt
	echo "Test dla 4 wątków" >> Times.txt
	./main 4 block $(pic1) res.pgm >> Times.txt
	./main 4 block $(pic2) res.pgm >> Times.txt
	./main 4 block $(pic3) res.pgm >> Times.txt
	./main 4 block $(pic4) res.pgm >> Times.txt
	echo "Test dla 8 wątków" >> Times.txt
	./main 8 block $(pic1) res.pgm >> Times.txt
	./main 8 block $(pic2) res.pgm >> Times.txt
	./main 8 block $(pic3) res.pgm >> Times.txt
	./main 8 block $(pic4) res.pgm >> Times.txt
	echo "Test dla 16 wątków" >> Times.txt
	./main 16 block $(pic1) res.pgm >> Times.txt
	./main 16 block $(pic2) res.pgm >> Times.txt
	./main 16 block $(pic3) res.pgm >> Times.txt
	./main 16 block $(pic4) res.pgm >> Times.txt
	echo "Test dla 64 wątków" >> Times.txt
	./main 64 block $(pic1) res.pgm >> Times.txt
	./main 64 block $(pic2) res.pgm >> Times.txt
	./main 64 block $(pic3) res.pgm >> Times.txt
	./main 64 block $(pic4) res.pgm >> Times.txt
