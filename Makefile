.PHONY: all clean

all: mierniczyc mierniczys

mierniczyc: mierniczyc.c
	gcc mierniczyc.c -o mierniczyc

mierniczys: mierniczys.c
	gcc mierniczys.c -o mierniczys

clean:
	rm -rf mierniczyc mierniczys
