CC = g++
SDSL_FLAGS = -lsdsl

all: prog

prog: main.o

main.o: inv_index.h
	$(CC) -o lab5 main.cpp $(SDSL_FLAGS)

clean:
	rm lab5 *.o
