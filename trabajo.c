#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "colaEnteros.h"
#include "minMonticulo.h"

#define MAX_COLUMNS 150
#define NUM_STATS 13
#define BUFFER_SIZE 2048

// Índices de las columnas en el CSV que nos interesan
const int COL_INDICES[NUM_STATS] = {12, 13, 20, 22, 36, 50, 56, 60, 83, 109, 110, 114, 112};
// Nombres para mostrar al usuario
const char *STAT_NAMES[NUM_STATS] = {
    "Goals", "Shots", "ShoPk", "PasTotCmp", "Assists", "CK", 
    "PassBlocks", "ScaDrib", "Int", "CrdY", "CrdR", "Off", "Fls"
};

// Estructura para agrupar todas las colas y datos del sistema
typedef struct {
    tipoCola colasRaw[NUM_STATS];   // Colas con datos originales
    tipoCola colasNorm[NUM_STATS];  // Colas con datos normalizados
    tipoCola colaPos;               // Cola de posiciones
    float medias[NUM_STATS];        // Medias calculadas
    float minimos[NUM_STATS];       // Mínimos detectados
    float maximos[NUM_STATS];       // Máximos detectados
} DatosJugadores;

// --- Funciones Auxiliares ---

int obtenerNumeroPosicion(char *posicion) {
    char *posiciones[] = {"DF", "GK", "FW", "MF"};
    int numeros[] = {1, 2, 3, 4};
    
    while (*posicion == ' ' || *posicion == '\t') posicion++; // Trim espacios
    
    for (int i = 0; i < 4; i++) {
        if (strcmp(posicion, posiciones[i]) == 0) return numeros[i];
    }
    return 0;
}

float normalizar(float valor, float minimo, float maximo) {
    if ((maximo - minimo) != 0) {
        return (valor - minimo) / (maximo - minimo);
    }
    return valor;
}

// --- Gestión de Memoria y Estructuras ---

void inicializarSistema(DatosJugadores *datos) {
    nuevaCola(&datos->colaPos);
    for (int i = 0; i < NUM_STATS; i++) {
        nuevaCola(&datos->colasRaw[i]);
        nuevaCola(&datos->colasNorm[i]);
        datos->medias[i] = 0;
    }
}

// --- Lectura de Archivo ---

int leerCSV(const char *filename, DatosJugadores *datos) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return 0;
    }

    char line[BUFFER_SIZE];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        if (lineNumber == 1) continue; // Saltar cabecera

        // Hacemos una copia de la línea para strtok
        char *token;
        char *columnas[MAX_COLUMNS];
        int colCount = 0;

        token = strtok(line, ";");
        while (token != NULL && colCount < MAX_COLUMNS) {
            columnas[colCount++] = token;
            token = strtok(NULL, ";");
        }

        // Procesar posición (Columna 3)
        if (colCount >= 4) {
            int numPos = obtenerNumeroPosicion(columnas[3]);
            if (numPos != 0) encolar(&datos->colaPos, (float)numPos);
        }

        // Procesar Estadísticas
        // Verificamos que la línea tenga suficientes columnas para el índice más alto
        if (colCount > 114) { 
            for (int i = 0; i < NUM_STATS; i++) {
                int colIndex = COL_INDICES[i];
                encolar(&datos->colasRaw[i], atoi(columnas[colIndex]));
            }
        }
    }
    fclose(file);
    return 1;
}

// --- Procesamiento de Datos ---

void procesarYNormalizar(DatosJugadores *datos) {
    for (int i = 0; i < NUM_STATS; i++) {
        float suma = 0;
        int count = 0;
        
        // Guardamos min y max actuales de la cola raw para usarlos al pedir input
        datos->minimos[i] = datos->colasRaw[i].minimo;
        datos->maximos[i] = datos->colasRaw[i].maximo;

        while (!esNulaCola(datos->colasRaw[i])) {
            float valor = frente(datos->colasRaw[i]);
            desencolar(&datos->colasRaw[i]);
            
            suma += valor;
            count++;
            
            // Normalizar y guardar en la cola secundaria
            float valNorm = normalizar(valor, datos->minimos[i], datos->maximos[i]);
            encolar(&datos->colasNorm[i], valNorm);
        }
        
        datos->medias[i] = (count > 0) ? (suma / count) : 0;
    }
}

// --- Interacción con Usuario ---

void obtenerInputUsuario(DatosJugadores *datos, float *inputUsuario) {
    printf("Introduce los valores del jugador a predecir.\n");
    printf("Si no los conoces, introduce -1 para usar la media sugerida.\n\n");

    for (int i = 0; i < NUM_STATS; i++) {
        float val;
        printf("%s (min=%.2f, max=%.2f, mean=%.2f): ", 
               STAT_NAMES[i], datos->minimos[i], datos->maximos[i], datos->medias[i]);
        scanf("%f", &val);
        
        if (val == -1) val = datos->medias[i];
        
        // Normalizamos el input del usuario inmediatamente para compararlo luego
        inputUsuario[i] = normalizar(val, datos->minimos[i], datos->maximos[i]);
    }
}

// --- Algoritmo KNN ---

void calcularDistancias(DatosJugadores *datos, float *inputUsuario, tipoMinMonticulo *monticulo) {
    // Asumimos que todas las colas tienen la misma longitud
    while (!esNulaCola(datos->colasNorm[0]) && !esNulaCola(datos->colaPos)) {
        float sumatorio = 0;

        for (int i = 0; i < NUM_STATS; i++) {
            if (esNulaCola(datos->colasNorm[i])) break; // Seguridad
            
            float valJugador = frente(datos->colasNorm[i]);
            desencolar(&datos->colasNorm[i]);

            float diff = valJugador - inputUsuario[i];
            sumatorio += (diff * diff);
        }

        float distancia = sqrt(sumatorio);
        float posicion = frente(datos->colaPos);
        desencolar(&datos->colaPos);

        tipoElementoMinMonticulo elemento;
        elemento.numero = distancia;
        elemento.posicion = (int)posicion;
        insertarMinMonticulo(monticulo, elemento);
    }
}

void predecirPosicion(tipoMinMonticulo *monticulo) {
    int K;
    printf("\nIntroduce K (numero de vecinos a considerar, ej: 3): ");
    scanf("%d", &K);
    if (K < 1) K = 1;

    int votes[5] = {0}; // Índices 1-4 usados
    float sumDist[5] = {0};

    for (int i = 0; i < K; i++) {
        if (esVacio(*monticulo)) break;
        tipoElementoMinMonticulo res = extraerMin(monticulo);
        int pos = (int)res.posicion;
        
        if (pos >= 1 && pos <= 4) {
            votes[pos]++;
            sumDist[pos] += res.numero;
        }
    }

    // Determinar ganador
    int bestPos = -1;
    int bestVotes = -1;
    float bestAvgDist = 1e30;

    for (int p = 1; p <= 4; p++) {
        if (votes[p] > bestVotes) {
            bestVotes = votes[p];
            bestPos = p;
            bestAvgDist = (votes[p] > 0) ? (sumDist[p] / votes[p]) : 1e30;
        } else if (votes[p] == bestVotes && votes[p] > 0) {
            // Desempate por distancia promedio
            float avg = sumDist[p] / votes[p];
            if (avg < bestAvgDist) {
                bestPos = p;
                bestAvgDist = avg;
            }
        }
    }

    char *nombresPos[] = {"?", "DF", "GK", "FW", "MF"};
    if (bestPos >= 1 && bestPos <= 4) {
        printf("\n>>> La posición predicha es: %s <<<\n", nombresPos[bestPos]);
    } else {
        printf("\nNo se ha podido determinar la posición.\n");
    }
}

// --- Main ---

int main() {
    DatosJugadores datos;
    tipoMinMonticulo monticulo;
    float inputUsuario[NUM_STATS];

    // 1. Inicialización
    inicializarSistema(&datos);
    nuevoMinMonticulo(&monticulo, 2000); // Asumiendo tamaño suficiente

    // 2. Carga de datos
    if (!leerCSV("2022-2023 Football Player Stats.csv", &datos)) {
        return 1;
    }

    // 3. Procesamiento (Normalización y Medias)
    procesarYNormalizar(&datos);

    // 4. Input del usuario
    obtenerInputUsuario(&datos, inputUsuario);

    // 5. Cálculo KNN
    calcularDistancias(&datos, inputUsuario, &monticulo);

    // 6. Resultado
    predecirPosicion(&monticulo);

    return 0;
}