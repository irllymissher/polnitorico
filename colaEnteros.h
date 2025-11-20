#include <stdbool.h>

typedef float tipoElementoCola;
typedef struct celdaC{
	tipoElementoCola elem;
	struct celdaC* sig;
} celdaCola; 
typedef struct tipoC{
	celdaCola* ini;
	celdaCola* fin;
	tipoElementoCola maximo;
	tipoElementoCola minimo;
}tipoCola;

typedef struct celdaCC{
	tipoCola cola;
	struct celdaCC* sig;
} celdaColaC; 

typedef struct tipoCC{
    celdaColaC* ini;
    celdaColaC* fin;
} tipoColaDeColas;

void nuevaCola(tipoCola*);

bool esNulaCola(tipoCola);

void encolar(tipoCola*, tipoElementoCola);

void desencolar(tipoCola*);

tipoElementoCola frente(tipoCola);

void nuevaColaDeColas(tipoColaDeColas*);

bool esNulaColaDeColas(tipoColaDeColas);

void encolarCola(tipoColaDeColas* , tipoCola);

void desencolarCola(tipoColaDeColas* );

tipoCola frenteColaDeColas(tipoColaDeColas ); 



