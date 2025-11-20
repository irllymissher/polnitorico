#include "minMonticulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void nuevoMinMonticulo(tipoMinMonticulo* m, int max)
{
    m->array = (tipoElementoMinMonticulo*)malloc(max * sizeof(tipoElementoMinMonticulo));
    m->pos = 0;
    m->numEl = max;
}

void insertarMinMonticulo(tipoMinMonticulo *m, tipoElementoMinMonticulo e)
{
    if(estaLleno(*m))
    {
        printf("El montículo está lleno, no se puede insertar.\n");
        return;
    }
    else
    {
        int i = m->pos;
       (m->array)[i] = e;
        m->pos++;
        while (i > 0 && (m->array)[i].numero < (m->array)[(i - 1) / 2].numero)
        {
            tipoElementoMinMonticulo aux;
            aux = (m->array)[i];
            (m->array)[i] = (m->array)[(i - 1) / 2];
            (m->array)[(i - 1) / 2] = aux;
            i = (i - 1) / 2;
        }
    }
}

void eliminarElemento(tipoMinMonticulo *m, tipoElementoMinMonticulo e)
{
    if (m->pos == 0) {
        printf("El montículo está vacío, no se puede eliminar.\n");
        return;
    }

    int i;
    for (i = 0; i < m->pos; i++) {
        if ((m->array)[i].numero == e.numero) {
            break;
        }
    }

    if (i == m->pos) {
        printf("Elemento no encontrado en el montículo.\n");
        return;
    }

    (m->array)[i] = (m->array)[m->pos - 1];
    m->pos--;

    int indice = i;
    while (indice < m->pos) {
        int izquierda = (indice * 2) + 1;
        int derecha = (indice * 2) + 2;
        int menor = indice;

        if (izquierda < m->pos && (m->array)[izquierda].numero < (m->array)[menor].numero)
        {
            menor = izquierda;
        }

        if (derecha < m->pos && (m->array)[derecha].numero < (m->array)[menor].numero)
        {
            menor = derecha;
        }

        if (menor != indice)
        {
            tipoElementoMinMonticulo aux;
            aux = (m->array)[indice];
            (m->array)[indice] = (m->array)[menor];
            (m->array)[menor] = aux;
            indice = menor;
        }
        else
        {
            break;
        }
    }
}

tipoElementoMinMonticulo devolverRaiz(tipoMinMonticulo m)
{
    if (esVacio(m))
    {
        printf("El montículo está vacío.\n");
        tipoElementoMinMonticulo sentinela = {-1, -1};
        return sentinela; // Valor sentinela o error
    }

    return (m.array)[0];
}

tipoElementoMinMonticulo extraerMin(tipoMinMonticulo *m)
{
    if (esVacio(*m))
    {
        printf("El montículo está vacío.\n");
        tipoElementoMinMonticulo sentinela = {-1, -1};
        return sentinela;
    }

    tipoElementoMinMonticulo root = (m->array)[0];
    (m->array)[0] = (m->array)[m->pos - 1];
    m->pos--;

    int indice = 0;
    while (1)
    {
        int izquierda = (indice * 2) + 1;
        int derecha = (indice * 2) + 2;
        int menor = indice;

        if (izquierda < m->pos && (m->array)[izquierda].numero < (m->array)[menor].numero)
        {
            menor = izquierda;
        }

        if (derecha < m->pos && (m->array)[derecha].numero < (m->array)[menor].numero)
        {
            menor = derecha;
        }

        if (menor != indice)
        {
            tipoElementoMinMonticulo aux = (m->array)[indice];
            (m->array)[indice] = (m->array)[menor];
            (m->array)[menor] = aux;
            indice = menor;
        }
        else
        {
            break;
        }
    }

    return root;
}

void mostrarAnchura(tipoMinMonticulo m)
{
    if (esVacio(m))
    {
        printf("El montículo está vacío.\n");
        return;
    }

    printf("Montículo en anchura: ");

    for (int i = 0; i < m.pos; i++)
    {
        printf("%.5f, %d ", (m.array)[i].numero, (m.array)[i].posicion);
    }

    printf("\n");
}

bool esVacio(tipoMinMonticulo m)
{
    return m.pos == 0;
}

bool estaLleno(tipoMinMonticulo m)
{
    return m.pos == m.numEl;
}
