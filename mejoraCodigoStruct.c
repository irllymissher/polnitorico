// 1. Nuevas constantes y estructura (idealmente en un header)
#define NUM_STATS 13

// Definición de índices para acceder al array de stats
enum StatIndex {
    IDX_GOALS = 0, IDX_SHOTS, IDX_SHO_PK, IDX_PAS_TOT_CMP, IDX_ASSISTS, 
    IDX_CK, IDX_PAS_BLOCKS, IDX_SCA_DRIB, IDX_INT, IDX_CRDY, IDX_CRDR, 
    IDX_OFF, IDX_FLS
};

typedef struct {
    float stats_normalizadas[NUM_STATS];
    int posicion_real; // 1=DF, 2=GK, 3=FW, 4=MF
} tipoJugador;

// NOTA: Para este ejemplo, mantendremos las colas originales (ColaGoals, etc.) 
//       para la PRIMERA PASADA (Min/Max). La ColaFinal será 'ColaJugadores'.
//       Asumimos que 'tipoCola' soporta floats y 'tipoColaJugador' soporta 'tipoJugador'.

int main()
{
    // ... (Inicialización de variables y apertura de archivo) ...

    // Colas TEMPORALES para almacenar los valores brutos y calcular Min/Max
    tipoCola ColaRaw[NUM_STATS]; // Array de colas para datos brutos
    tipoCola ColaPosRaw;          // Cola para posiciones brutas
    tipoCola ColaFinalJugadores;  // LA ÚNICA COLA DE JUGADORES REFRACTORIZADA

    // Arrays para guardar los valores de normalización (Min/Max de todo el dataset)
    float min_values[NUM_STATS];
    float max_values[NUM_STATS];
    
    // Inicialización de colas y Min/Max. (Se necesitaría inicializar las ColasRaw[i])
    for (int i = 0; i < NUM_STATS; i++) {
        nuevaCola(&ColaRaw[i]);
        min_values[i] = 1e30; // Inicializar a valor grande
        max_values[i] = -1e30; // Inicializar a valor pequeño
    }
    nuevaCola(&ColaPosRaw);
    // Asumimos que esta es la nueva cola que almacena structs de jugadores
    nuevaColaJugador(&ColaFinalJugadores); 
    
    // Arrays para mapear las columnas del CSV a los índices del struct
    int csv_columns[NUM_STATS] = {12, 13, 20, 22, 36, 50, 56, 60, 83, 109, 110, 114, 112};
    
    // --- PRIMERA PASADA: Carga y Extracción de Min/Max ---
    // (Tu código actual ya hace esto en el primer loop, pero lo simplificamos)
    
    FILE *file = fopen("2022-2023 Football Player Stats.csv", "r");
    // ... (manejo de error) ...

    // Bucle para leer y cargar los datos brutos.
    // (Este bucle se parece a tu actual, pero encola en las colas temporales)
    while (fgets(line, sizeof(line), file) != NULL) {
        // ... (Tokenización de la línea) ...
        
        // Carga de Posición
        if (cont >= 4) {
            int numPos = obtenerNumeroPosicion(columnas[3]);
            if (numPos != 0) {
                encolar(&ColaPosRaw, (float)numPos);
            }
        }

        // Carga de Estadísticas, actualizando Min/Max al mismo tiempo
        if (cont >= 120) {
            for (int i = 0; i < NUM_STATS; i++) {
                float valor = (float)atoi(columnas[csv_columns[i]]);
                encolar(&ColaRaw[i], valor);
                
                // Actualizar Min/Max
                if (valor < min_values[i]) min_values[i] = valor;
                if (valor > max_values[i]) max_values[i] = valor;
            }
        }
    }
    fclose(file);
    
    // --- SEGUNDA PASADA: Normalización y Creación de la Cola Única ---
    // Recorremos las colas temporales y creamos el struct Jugador.
    
    while (!esNulaCola(&ColaPosRaw)) {
        tipoJugador nuevoJugador;
        float pos = frente(&ColaPosRaw);
        desencolar(&ColaPosRaw);
        
        nuevoJugador.posicion_real = (int)pos;

        // Normalizar los 13 datos y guardarlos en el array del struct
        for (int i = 0; i < NUM_STATS; i++) {
            float raw_value = frente(&ColaRaw[i]);
            desencolar(&ColaRaw[i]);
            
            float min = min_values[i];
            float max = max_values[i];
            
            // Aplicar la normalización y guardar en el struct
            nuevoJugador.stats_normalizadas[i] = normalizar(raw_value, min, max);
        }
        
        // ¡Solo un encolar!
        encolarJugador(&ColaFinalJugadores, nuevoJugador);
    }
    
    // ... (El resto del main se simplifica enormemente) ...
    
    // --- CÁLCULO DE DISTANCIA (Mucho más limpio) ---
    // Necesitarás las 13 variables de input (Goals, Shots, etc.) y un array de input
    float input_stats[NUM_STATS] = {Goals, Shots, ...}; // Rellenado con el input del usuario

    // Bucle de cálculo:
    while (!esNulaColaJugador(&ColaFinalJugadores)) {
        tipoJugador jugadorArchivo = frenteJugador(&ColaFinalJugadores);
        desencolarJugador(&ColaFinalJugadores);
        
        sumatorio = 0;
        for (int i = 0; i < NUM_STATS; i++) {
            // Normalizar el input del usuario
            float normalized_input = normalizar(input_stats[i], min_values[i], max_values[i]);
            
            // Cálculo de distancia (Euclidiana)
            float diff = jugadorArchivo.stats_normalizadas[i] - normalized_input;
            sumatorio += diff * diff;
        }
        distancia = sqrt(sumatorio);

        res.numero = distancia;
        res.posicion = jugadorArchivo.posicion_real;
        insertarMinMonticulo(&monticulo, res);
    }
    
    // ... (El resto del código de votación K-NN sigue igual) ...
    
    return 0;
}
