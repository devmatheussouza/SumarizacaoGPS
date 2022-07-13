CFLAGS = -g -Wall -std=c99

all: gps

gps: gps.o lib-estrutural.o funcoes-auxiliares.o
	gcc gps.o lib-estrutural.o funcoes-auxiliares.o -o gps

gps.o: gps.c lib-estrutural.h
	gcc $(CFLAGS) -c gps.c

lib-estrutural.o: lib-estrutural.c lib-estrutural.h 
	gcc $(CFLAGS) -c lib-estrutural.c

funcoes-auxiliares.o: funcoes-auxiliares.c funcoes-auxiliares.h
	gcc $(CFLAGS) -c funcoes-auxiliares.c

clean:
	-rm -f *.o *~ 

purge: clean
	-rm -f gps

