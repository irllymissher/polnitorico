#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "colaEnteros.h"
#include "minMonticulo.h"

#define MAX_COLUMNS 150
#define NUM_STATS 13
#define BUFFER_SIZE 2048
#define MAX_PLAYERS 1000

// Índices de las columnas en el CSV que nos interesan
const int COL_INDICES[NUM_STATS] = {12, 13, 20, 22, 36, 50, 56, 60, 83, 109, 110, 114, 112};
// Nombres para mostrar al usuario
const char *STAT_NAMES[NUM_STATS] = {
    "Goals", "Shots", "ShoPk", "PasTotCmp", "Assists", "CK", 
    "PassBlocks", "ScaDrib", "Int", "CrdY", "CrdR", "Off", "Fls"
};

const char *POS_NAMES[] = {"", "DF", "GK", "FW", "MF"}; // Mapeo de 1 a 4

// --- ALMACENAMIENTO GLOBAL DE JUGADORES (Para Justificación) ---
// Usaremos estas estructuras para mapear el índice del montículo al nombre y posición.
char nombresJugadores[MAX_PLAYERS][50];
int posicionesJugadores[MAX_PLAYERS]; // 1: DF, 2: GK, 3: FW, 4: MF
int totalJugadores = 0;

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

int obtenerInputUsuario_Batch(DatosJugadores *datos, float *inputUsuario) {
    char inputLine[BUFFER_SIZE];
    float valoresRaw[NUM_STATS];
    int readCount = 0;
    
    printf("\n--- ENTRADA EN BLOQUE ---\n");
    printf("Introduce los %d valores del jugador separados por espacios o comas:\n", NUM_STATS);
    printf("Orden: ");
    for(int i = 0; i < NUM_STATS; i++) {
        printf("%s%s", STAT_NAMES[i], (i == NUM_STATS - 1) ? "" : ", ");
    }
    printf("\nValores (usa -1 para la media sugerida):\n> ");

    // Leemos la línea completa
    if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) {
        printf("Error de lectura.\n");
        return 0;
    }

    // Usamos sscanf para intentar leer los 13 valores
    // Nota: sscanf es limitado y solo funcionará bien con espacios, no comas. 
    // Para comas, se necesitaría strtok o un parser más avanzado, pero sscanf es suficiente para una mejora rápida.
    readCount = sscanf(inputLine, "%f %f %f %f %f %f %f %f %f %f %f %f %f",
                       &valoresRaw[0], &valoresRaw[1], &valoresRaw[2], &valoresRaw[3], 
                       &valoresRaw[4], &valoresRaw[5], &valoresRaw[6], &valoresRaw[7],
                       &valoresRaw[8], &valoresRaw[9], &valoresRaw[10], &valoresRaw[11], &valoresRaw[12]);

    if (readCount != NUM_STATS) {
        printf("Error: Se leyeron %d valores, se esperaban %d. Por favor, inténtalo de nuevo.\n", readCount, NUM_STATS);
        return 0;
    }

    printf("\nDatos introducidos:\n");
    for (int i = 0; i < NUM_STATS; i++) {
        float val = valoresRaw[i];
        if (val == -1) val = datos->medias[i];
        
        printf("- %s: %.2f (Normalizado: %.4f)\n", 
               STAT_NAMES[i], val, normalizar(val, datos->minimos[i], datos->maximos[i]));
               
        inputUsuario[i] = normalizar(val, datos->minimos[i], datos->maximos[i]);
    }
    
    return 1;
}

// --- Algoritmo KNN ---

void calcularDistancias(DatosJugadores *datos, float *inputUsuario, tipoMinMonticulo *monticulo) {
    for (int idx = 0; idx < totalJugadores; idx++) {
        float sumatorio = 0;

        // Si alguna cola está vacía, algo salió mal en la carga, terminamos.
        if (esNulaCola(datos->colasNorm[0])) break;

        for (int i = 0; i < NUM_STATS; i++) {
            float valJugador = frente(datos->colasNorm[i]);
            desencolar(&datos->colasNorm[i]);

            float diff = valJugador - inputUsuario[i];
            sumatorio += (diff * diff);
        }

        float distancia = sqrt(sumatorio);

        // MEJORA: Almacenamos el ÍNDICE (idx) del jugador en lugar de la posición numérica.
        tipoElementoMinMonticulo elemento;
        elemento.numero = distancia;
        elemento.posicion = idx; // Usamos 'posicion' para guardar el índice
        insertarMinMonticulo(monticulo, elemento);
    }
}

// --- MEJORA 2 & 3: Justificación y ASCII Art ---

void mostrarVotosASCII(int *votes) {
    printf("\nDistribución de Votos (ASCII Art):\n");
    int maxVotes = 0;
    for (int i = 1; i <= 4; i++) {
        if (votes[i] > maxVotes) maxVotes = votes[i];
    }
    
    for (int p = 1; p <= 4; p++) {
        printf("%s: ", POS_NAMES[p]);
        // Escalar el arte a un máximo de 15 '#'
        int numHashes = (votes[p] > 0) ? (int)(((float)votes[p] / maxVotes) * 15.0) : 0;
        if (votes[p] > 0 && numHashes == 0) numHashes = 1; // Asegurar al menos 1 hash si hay votos

        for (int h = 0; h < numHashes; h++) {
            printf("#");
        }
        printf(" (%d votos)\n", votes[p]);
    }
}
void predecirPosicion(tipoMinMonticulo *monticulo) {
    int K;
	printf("\n============================================\n");
    printf("Introduce K (numero de vecinos a considerar): ");
    scanf("%d", &K);
	while (getchar() != '\n');
    if (K < 1) K = 1;
	
    int votes[5] = {0}; // Índices 1-4 usados
    float sumDist[5] = {0};

	tipoElementoMinMonticulo vecinosCercanos[K];
    int vecinosCount = 0;
	
    for (int i = 0; i < K; i++) {
        if (esVacio(*monticulo)) break;
        tipoElementoMinMonticulo res = extraerMin(monticulo);
        int pos = (int)res.posicion;
        int posv = posicionesJugadores[i];
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

	// --- Salida de Resultados ---
    char *nombresPos[] = {"?", "DF", "GK", "FW", "MF"};
    if (bestPos >= 1 && bestPos <= 4) {
        printf("\n>>> La posición predicha es: %s <<<\n", nombresPos[bestPos]);
    } else {
        printf("\nNo se ha podido determinar la posición.\n");
    }

	// MEJORA 2: ASCII Art
    mostrarVotosASCII(votes);

	// MEJORA 3: Mostrar Vecinos Cercanos
    printf("\n--- Top %d Vecinos Más Cercanos (Justificación) ---\n", vecinosCount);
    for(int i = 0; i < vecinosCount; i++) {
        int idx = vecinosCercanos[i].posicion;
        int posv = posicionesJugadores[idx];
        printf("%d. %s (%s) - Distancia Euclidiana: %.4f\n", 
               i + 1, 
               nombresJugadores[idx], 
               POS_NAMES[posv], 
               vecinosCercanos[i].numero);
    }
    printf("============================================\n");
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
    obtenerInputUsuario_Batch(&datos, inputUsuario);

    // 5. Cálculo KNN
    calcularDistancias(&datos, inputUsuario, &monticulo);

    // 6. Resultado
    predecirPosicion(&monticulo);

    return 0;
}