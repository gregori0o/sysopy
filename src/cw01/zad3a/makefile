cc = gcc -Wall -g

all: all_test

static_library:
	$(cc) -c merge_text.c -o merge_text.o
	ar rcs libmerge_text.a merge_text.o

shared_library:
	$(cc) -c -fPIC merge_text.c -o merge_text.o
	$(cc) -shared -o libmerge_text.so merge_text.o

clean:
	rm -f *.out *.so *.o *.a main

all_test:
	echo "Raport zadania 3a" > results3a.txt
	make static_test
	make shared_test
	make dynamic_test

static_test: static_library
	$(cc) -c main.c -o main.o
	$(cc) main.o libmerge_text.a -o main
	echo "Static library" >> results3a.txt
	make tests
	make clean

shared_test: shared_library
	$(cc) main.c -L./ -o main -lmerge_text -Wl,-rpath=.
	echo "Shared library" >> results3a.txt
	make tests
	make clean

dynamic_test: shared_library
	$(cc) main_dyn.c -ldl -o main -Wl,-rpath=.
	echo "Dynamic library" >> results3a.txt
	make tests
	make clean

tests:
	make small
	make medium
	make big
	make remove_and_add

small:
	echo "Testy dla małej liczby par (4), najpierw krótkie (ok.100), później średnie (ok.10000) i na końcu długie (ok.1000000)" >> results3a.txt
	./main create_table 4 merge_files "100l.txt:100l.txt 100l.txt:110l.txt 110l.txt:100l.txt 110l.txt:110l.txt" save_merge remove_all >> results3a.txt
	./main create_table 4 merge_files "1000l.txt:1000l.txt 1000l.txt:1100l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt" save_merge remove_all >> results3a.txt
	./main create_table 4 merge_files "100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" save_merge remove_all >> results3a.txt

medium:
	echo "Testy dla średniej liczby par (9), najpierw krótkie (ok.100), później średnie (ok.10000) i na końcu długie (ok.1000000)" >> results3a.txt
	./main create_table 9 merge_files "90l.txt:90l.txt 90l.txt:100l.txt 90l.txt:110l.txt 100l.txt:90l.txt 100l.txt:100l.txt 100l.txt:110l.txt 110l.txt:90l.txt 110l.txt:100l.txt 110l.txt:110l.txt" save_merge remove_all >> results3a.txt
	./main create_table 9 merge_files "900l.txt:900l.txt 900l.txt:1000l.txt 900l.txt:1100l.txt 1000l.txt:900l.txt 1000l.txt:1000l.txt 1000l.txt:1100l.txt 1100l.txt:900l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt" save_merge remove_all >> results3a.txt
	./main create_table 9 merge_files "100000l.txt:100000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 100000l.txt:100000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" save_merge remove_all >> results3a.txt

big:
	echo "Testy dla dużej liczby par (25), najpierw krótkie (ok.100), później średnie (ok.10000) i na końcu długie (ok.1000000)" >> results3a.txt
	./main create_table 25 merge_files "90l.txt:90l.txt 90l.txt:100l.txt 90l.txt:110l.txt 90l.txt:120l.txt 90l.txt:130l.txt 100l.txt:90l.txt 100l.txt:100l.txt 100l.txt:110l.txt 100l.txt:120l.txt 100l.txt:130l.txt 110l.txt:90l.txt 110l.txt:100l.txt 110l.txt:110l.txt 110l.txt:120l.txt 110l.txt:130l.txt 120l.txt:90l.txt 120l.txt:100l.txt 120l.txt:110l.txt 120l.txt:120l.txt 120l.txt:130l.txt 130l.txt:90l.txt 130l.txt:100l.txt 130l.txt:110l.txt 130l.txt:120l.txt 130l.txt:130l.txt" save_merge remove_all >> results3a.txt
	./main create_table 25 merge_files "900l.txt:900l.txt 900l.txt:1000l.txt 900l.txt:1100l.txt 900l.txt:1200l.txt 900l.txt:1300l.txt 1000l.txt:900l.txt 1000l.txt:1000l.txt 1000l.txt:1100l.txt 1000l.txt:1200l.txt 1000l.txt:1300l.txt 1100l.txt:900l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt 1100l.txt:1200l.txt 1100l.txt:1300l.txt 1200l.txt:900l.txt 1200l.txt:1000l.txt 1200l.txt:1100l.txt 1200l.txt:1200l.txt 1200l.txt:1300l.txt 1300l.txt:900l.txt 1300l.txt:1000l.txt 1300l.txt:1100l.txt 1300l.txt:1200l.txt 1300l.txt:1300l.txt" save_merge remove_all >> results3a.txt
	./main create_table 25 merge_files "100000l.txt:100000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 100000l.txt:120000l.txt 100000l.txt:130000l.txt 100000l.txt:100000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 100000l.txt:120000l.txt 100000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 130000l.txt:120000l.txt 130000l.txt:130000l.txt 120000l.txt:100000l.txt 120000l.txt:100000l.txt 120000l.txt:130000l.txt 120000l.txt:120000l.txt 120000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 130000l.txt:120000l.txt 130000l.txt:130000l.txt" save_merge remove_all >> results3a.txt

remove_and_add:
	echo "Testy dodawania i usuwania bloków nad 9 parami średniej długości tekstu wykonane najpierw 5 prób, następnie 10 i na koniec 20" >> results3a.txt
	./main create_table 9 merge_files "900l.txt:900l.txt 900l.txt:1000l.txt 900l.txt:1100l.txt 1000l.txt:900l.txt 1000l.txt:1000l.txt 1000l.txt:1100l.txt 1100l.txt:900l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt" remove_and_add_blocks 5 remove_and_add_blocks 10 remove_and_add_blocks 20 remove_all >> results3a.txt
