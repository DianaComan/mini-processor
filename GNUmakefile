all: build

build: so-cpp

so-cpp: tema.o
		gcc -Wall -g tema.o -o so-cpp
	
tema.o: tema.c
		gcc -Wall -c tema.c

clean:
		rm -rf *.o so-cpp
