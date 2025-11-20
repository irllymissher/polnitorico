all: trabajo
	./trabajo
trabajo: trabajo.c colaEnteros.o minMonticulo.o
	gcc -g trabajo.c colaEnteros.o minMonticulo.o -o trabajo -lm
colaEnteros.o: colaEnteros.c 
	gcc -g -c colaEnteros.c
minMonticulo.o: minMonticulo.c
	gcc -g -c minMonticulo.c
