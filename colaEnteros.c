#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "colaEnteros.h"

void nuevaCola(tipoCola* c) {
    c->ini = NULL;
    c->fin = NULL;
    c->maximo = 0; 
    c->minimo = 0; 
}

bool esNulaCola(tipoCola c) {
    return (c.ini == NULL);
}

void encolar(tipoCola* c, tipoElementoCola e) {
    celdaCola* nuevo;
    nuevo = (celdaCola*)malloc(sizeof(celdaCola));
    nuevo->elem = e;
    nuevo->sig = NULL;
	
    if (c->fin == NULL) {
        c->maximo = e;
        c->minimo = e;
    } else {
        if (e > c->maximo) {
            c->maximo = e;
        }
        if (e < c->minimo) {
            c->minimo = e;
        }
    }

    if (c->fin == NULL) {
        c->ini = nuevo;
    } else {
        c->fin->sig = nuevo;
    }
    c->fin = nuevo;
}

void desencolar(tipoCola* c) {
    if (esNulaCola(*c)) {
        printf("\nERROR: No se puede desencolar de cola NULA\n");
        return;
    }

    celdaCola* aux;
    aux = c->ini;
    c->ini = c->ini->sig;
    if (c->ini == NULL) {
        c->fin = NULL;
    }
    free(aux);
}

tipoElementoCola frente(tipoCola c) {
    if (esNulaCola(c)) {
        printf("\nERROR: No existe frente de cola NULA\n");
    }
    return c.ini->elem;
}

void nuevaColaDeColas(tipoColaDeColas* cc) {
    cc->ini = NULL;
    cc->fin = NULL;
}

bool esNulaColaDeColas(tipoColaDeColas cc) {
    return (cc.ini == NULL);
}

void encolarCola(tipoColaDeColas* cc, tipoCola cola) {
    celdaColaC* nuevaCelda;
    nuevaCelda = (celdaColaC*)malloc(sizeof(celdaColaC));
    nuevaCelda->cola = cola;
    nuevaCelda->sig = NULL;
    if (cc->fin == NULL) {
        cc->ini = nuevaCelda;
    } else {
        cc->fin->sig = nuevaCelda;
    }
    cc->fin = nuevaCelda;
}

void desencolarCola(tipoColaDeColas* cc) {
    if (esNulaColaDeColas(*cc)) {
        printf("\nERROR: No se puede desencolar de cola de colas NULA\n");
        return;
    }
    celdaColaC* aux;
    aux = cc->ini;
    cc->ini = cc->ini->sig;
    if (cc->ini == NULL) {
        cc->fin = NULL;
    }
    free(aux);
}

tipoCola frenteColaDeColas(tipoColaDeColas cc) {
    if (esNulaColaDeColas(cc)) {
        printf("\nERROR: No existe frente de cola de colas NULA\n");
    }
    return cc.ini->cola;
}

// Mostrar cola
void mostrarCola(tipoCola c) {
    celdaCola* aux;
    aux = c.ini;
    while (aux != NULL) {
        printf("%f ", aux->elem);
        aux = aux->sig;
    }
    printf("\n");
}

// Tamaño de la cola
int tamanoCola(tipoCola c) {
    celdaCola* aux;
    aux = c.ini;
    int cont = 0;
    while (aux != NULL) {
        cont++;
        aux = aux->sig;
    }
    return cont;
}

