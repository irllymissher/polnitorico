#include <stdbool.h>

typedef struct tEMM{
	float numero;
	int posicion;
}tipoElementoMinMonticulo;

typedef struct tMM{
	tipoElementoMinMonticulo* array;
	int pos;
	int numEl;
}tipoMinMonticulo;

void nuevoMinMonticulo(tipoMinMonticulo* , int );

void insertarMinMonticulo(tipoMinMonticulo *, tipoElementoMinMonticulo );

void eliminarElemento(tipoMinMonticulo *, tipoElementoMinMonticulo );

tipoElementoMinMonticulo devolverRaiz(tipoMinMonticulo );
// devolver el elemento raiz del monticulo que será el elemento con menor valor

void mostrarAnchura(tipoMinMonticulo );

bool esVacio(tipoMinMonticulo );

bool estaLleno(tipoMinMonticulo );

// Extrae (y elimina) el elemento mínimo (raíz) del montículo
tipoElementoMinMonticulo extraerMin(tipoMinMonticulo *m);
