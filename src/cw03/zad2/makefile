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
	echo "Raport zadania 2" > raport2.txt
	make static_test
	make shared_test

static_test: static_library
	$(cc) -c main.c -o main.o
	$(cc) main.o libmerge_text.a -o main
	echo "Static library" >> raport2.txt
	make tests
	make clean

shared_test: shared_library
	$(cc) main.c -L./ -o main -lmerge_text -Wl,-rpath=.
	echo "Shared library" >> raport2.txt
	make tests
	make clean

tests:
	make small
	make medium
	make big

small:
	echo "Testy dla małej liczby par (4), najpierw krótkie (ok.100), później średnie (ok.10000) i na końcu długie (ok.1000000)" >> raport2.txt
	./main create_table 4 merge_files_one "100l.txt:100l.txt 100l.txt:110l.txt 110l.txt:100l.txt 110l.txt:110l.txt" remove_all >> raport2.txt
	./main create_table 4 merge_files_one "1000l.txt:1000l.txt 1000l.txt:1100l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt" remove_all >> raport2.txt
	./main create_table 4 merge_files_one "100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" remove_all >> raport2.txt
	./main create_table 4 merge_files_many "100l.txt:100l.txt 100l.txt:110l.txt 110l.txt:100l.txt 110l.txt:110l.txt" remove_all >> raport2.txt
	./main create_table 4 merge_files_many "1000l.txt:1000l.txt 1000l.txt:1100l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt" remove_all >> raport2.txt
	./main create_table 4 merge_files_many "100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" remove_all >> raport2.txt

medium:
	echo "Testy dla średniej liczby par (9), najpierw krótkie (ok.100), później średnie (ok.10000) i na końcu długie (ok.1000000)" >> raport2.txt
	./main create_table 9 merge_files_one "90l.txt:90l.txt 90l.txt:100l.txt 90l.txt:110l.txt 100l.txt:90l.txt 100l.txt:100l.txt 100l.txt:110l.txt 110l.txt:90l.txt 110l.txt:100l.txt 110l.txt:110l.txt" remove_all >> raport2.txt
	./main create_table 9 merge_files_one "900l.txt:900l.txt 900l.txt:1000l.txt 900l.txt:1100l.txt 1000l.txt:900l.txt 1000l.txt:1000l.txt 1000l.txt:1100l.txt 1100l.txt:900l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt" remove_all >> raport2.txt
	./main create_table 9 merge_files_one "90000l.txt:90000l.txt 90000l.txt:100000l.txt 90000l.txt:130000l.txt 100000l.txt:90000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:90000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" remove_all >> raport2.txt
	./main create_table 9 merge_files_many "90l.txt:90l.txt 90l.txt:100l.txt 90l.txt:110l.txt 100l.txt:90l.txt 100l.txt:100l.txt 100l.txt:110l.txt 110l.txt:90l.txt 110l.txt:100l.txt 110l.txt:110l.txt" remove_all >> raport2.txt
	./main create_table 9 merge_files_many "900l.txt:900l.txt 900l.txt:1000l.txt 900l.txt:1100l.txt 1000l.txt:900l.txt 1000l.txt:1000l.txt 1000l.txt:1100l.txt 1100l.txt:900l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt" remove_all >> raport2.txt
	./main create_table 9 merge_files_many "90000l.txt:90000l.txt 90000l.txt:100000l.txt 90000l.txt:130000l.txt 100000l.txt:90000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:90000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" remove_all >> raport2.txt

big:
	echo "Testy dla dużej liczby par (25), najpierw krótkie (ok.100), później średnie (ok.10000) i na końcu długie (ok.1000000)" >> raport2.txt
	./main create_table 25 merge_files_one "90l.txt:90l.txt 90l.txt:100l.txt 90l.txt:110l.txt 90l.txt:120l.txt 90l.txt:130l.txt 100l.txt:90l.txt 100l.txt:100l.txt 100l.txt:110l.txt 100l.txt:120l.txt 100l.txt:130l.txt 110l.txt:90l.txt 110l.txt:100l.txt 110l.txt:110l.txt 110l.txt:120l.txt 110l.txt:130l.txt 120l.txt:90l.txt 120l.txt:100l.txt 120l.txt:110l.txt 120l.txt:120l.txt 120l.txt:130l.txt 130l.txt:90l.txt 130l.txt:100l.txt 130l.txt:110l.txt 130l.txt:120l.txt 130l.txt:130l.txt" remove_all >> raport2.txt
	./main create_table 25 merge_files_one "900l.txt:900l.txt 900l.txt:1000l.txt 900l.txt:1100l.txt 900l.txt:1200l.txt 900l.txt:1300l.txt 1000l.txt:900l.txt 1000l.txt:1000l.txt 1000l.txt:1100l.txt 1000l.txt:1200l.txt 1000l.txt:1300l.txt 1100l.txt:900l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt 1100l.txt:1200l.txt 1100l.txt:1300l.txt 1200l.txt:900l.txt 1200l.txt:1000l.txt 1200l.txt:1100l.txt 1200l.txt:1200l.txt 1200l.txt:1300l.txt 1300l.txt:900l.txt 1300l.txt:1000l.txt 1300l.txt:1100l.txt 1300l.txt:1200l.txt 1300l.txt:1300l.txt" remove_all >> raport2.txt
	./main create_table 25 merge_files_one "90000l.txt:90000l.txt 90000l.txt:100000l.txt 90000l.txt:130000l.txt 90000l.txt:100000l.txt 90000l.txt:130000l.txt 100000l.txt:90000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:90000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 100000l.txt:90000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:90000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" remove_all >> raport2.txt
	./main create_table 25 merge_files_many "90l.txt:90l.txt 90l.txt:100l.txt 90l.txt:110l.txt 90l.txt:120l.txt 90l.txt:130l.txt 100l.txt:90l.txt 100l.txt:100l.txt 100l.txt:110l.txt 100l.txt:120l.txt 100l.txt:130l.txt 110l.txt:90l.txt 110l.txt:100l.txt 110l.txt:110l.txt 110l.txt:120l.txt 110l.txt:130l.txt 120l.txt:90l.txt 120l.txt:100l.txt 120l.txt:110l.txt 120l.txt:120l.txt 120l.txt:130l.txt 130l.txt:90l.txt 130l.txt:100l.txt 130l.txt:110l.txt 130l.txt:120l.txt 130l.txt:130l.txt" remove_all >> raport2.txt
	./main create_table 25 merge_files_many "900l.txt:900l.txt 900l.txt:1000l.txt 900l.txt:1100l.txt 900l.txt:1200l.txt 900l.txt:1300l.txt 1000l.txt:900l.txt 1000l.txt:1000l.txt 1000l.txt:1100l.txt 1000l.txt:1200l.txt 1000l.txt:1300l.txt 1100l.txt:900l.txt 1100l.txt:1000l.txt 1100l.txt:1100l.txt 1100l.txt:1200l.txt 1100l.txt:1300l.txt 1200l.txt:900l.txt 1200l.txt:1000l.txt 1200l.txt:1100l.txt 1200l.txt:1200l.txt 1200l.txt:1300l.txt 1300l.txt:900l.txt 1300l.txt:1000l.txt 1300l.txt:1100l.txt 1300l.txt:1200l.txt 1300l.txt:1300l.txt" remove_all >> raport2.txt
	./main create_table 25 merge_files_many "90000l.txt:90000l.txt 90000l.txt:100000l.txt 90000l.txt:130000l.txt 90000l.txt:100000l.txt 90000l.txt:130000l.txt 100000l.txt:90000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:90000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 100000l.txt:90000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 100000l.txt:100000l.txt 100000l.txt:130000l.txt 130000l.txt:90000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt 130000l.txt:100000l.txt 130000l.txt:130000l.txt" remove_all >> raport2.txt