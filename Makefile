.PHONY: all clean

CFLAGS+=-Wall -g

all: mierniczyc mierniczys

mierniczyc: err.o utils.o mierniczyc.c
	gcc $(CFLAGS) mierniczyc.c utils.o err.o -o mierniczyc

mierniczys: err.o utils.o mierniczys.c
	gcc $(CFLAGS) mierniczys.c utils.o err.o -o mierniczys

utils.o: utils.h utils.c err.h
	gcc $(CFLAGS) utils.c -c -o utils.o

err.o: err.h err.c
	gcc $(CFLAGS) err.c -c -o err.o

clean:
	rm -rf mierniczyc mierniczys *.o
