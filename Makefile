.PHONY: all clean

CXXFLAGS=-Wall -g

all: mierniczyc mierniczys

mierniczyc: mierniczyc.c
	gcc $(CXXFLAGS) mierniczyc.c -o mierniczyc

mierniczys: mierniczys.c
	gcc $(CXXFLAGS) mierniczys.c -o mierniczys

clean:
	rm -rf mierniczyc mierniczys
