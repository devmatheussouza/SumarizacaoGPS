CFLAGS = -g -Wall -std=c99

all: gps

gps: gps.o lib-estrutural.o funcoes-auxiliares.o manipula-logs.o interacao-usuario.o plota-grafico.o
	gcc gps.o lib-estrutural.o funcoes-auxiliares.o manipula-logs.o interacao-usuario.o plota-grafico.o -lm -o gps

gps.o: gps.c lib-estrutural.h
	gcc $(CFLAGS) -c gps.c

lib-estrutural.o: lib-estrutural.c lib-estrutural.h 
	gcc $(CFLAGS) -c lib-estrutural.c

funcoes-auxiliares.o: funcoes-auxiliares.c funcoes-auxiliares.h
	gcc $(CFLAGS) -c funcoes-auxiliares.c

manipula-logs.o: manipula-logs.c manipula-logs.h
	gcc $(CFLAGS) -c manipula-logs.c

interacao-usuario.o: interacao-usuario.c interacao-usuario.h
	gcc $(CFLAGS) -c interacao-usuario.c

plota-grafico.o: plota-grafico.c plota-grafico.h
	gcc $(CFLAGS) -c plota-grafico.c

clean:
	-rm -f *.o *~ 

purge: clean
	-rm -f gps

