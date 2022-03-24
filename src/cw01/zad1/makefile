cc = gcc -Wall -g

all: static_library shared_library

static_library:
	$(cc) -c merge_text.c -o merge_text.o
	ar rcs libmerge_text.a merge_text.o

shared_library:
	$(cc) -c -fPIC merge_text.c -o merge_text.o
	$(cc) -shared -o libmerge_text.so merge_text.o

clean:
	rm -f *.out *.so *.o *.a