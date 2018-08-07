.PHONY : all clean run new

MPIROOT = /usr

CFLAGS+= -Wall -std=c++11 -pthread -O3 -g $(INCL)
CC=gcc
MPICC=  $(MPIROOT)/bin/mpic++
INCL= -I$(MPIROOT)/include
SRCS= car.cpp distance.cpp
BIN= car


all: car

$(BIN): $(SRCS)
	$(MPICC) $(CFLAGS) -o  $(BIN) $(SRCS)

clean:
	rm -f $(BIN) *.o

new: clean all

run: car
	mpirun -np 4 ./car
