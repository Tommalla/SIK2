.PHONY: all clean

CFLAGS+=-Wall -g

all: mierniczyc mierniczys

mierniczyc: err.o mierniczyc.c
	gcc $(CFLAGS) mierniczyc.c err.o -o mierniczyc

mierniczys: err.o mierniczys.c
	gcc $(CFLAGS) mierniczys.c err.o -o mierniczys

err.o: err.h err.c
	gcc $(CFLAGS) err.c -c -o err.o

clean:
	rm -rf mierniczyc mierniczys *.o
