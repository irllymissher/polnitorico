# CAVAS (dudas)

1) Especificacion de cada dato estadistico para que el cliente sepa si puede hacerlo con un tipo de dato u otro.

## IDEA 
1)** Especificación de los datos (Ayuda al usuario)**
   ```bash
      void mostrarLeyenda() {
       printf("\n--- LEYENDA DE DATOS REQUERIDOS ---\n");
       printf("1. Goals: Goles totales (Entero >= 0)\n");
       printf("2. Shots: Tiros totales (Entero >= Goals)\n");
       printf("3. ShoPk: Tiros a puerta (Entero <= Shots)\n");
       printf("4. PasTotCmp: Pases completados (Entero)\n");
       // ... y así con el resto ...
       printf("-----------------------------------\n\n");
      }

// En el main, llamas a esta función antes de pedir datos.
   ```
2)** Mejora de Interfaz (Input en una sola línea)**
```
printf("Introduce los 13 datos SEPARADOS POR ESPACIOS en este orden:\n");
printf("[Goals Shots ShoPk PasTotCmp Assists CK PassBlocks ScaDrib Int CrdY CrdR Off Fls]\n");
printf("Ejemplo: 10 25 15 500 5 2 1 20 10 2 0 1 5\n");
printf(">> "); // Indicador visual para escribir

// Leemos todo de golpe. scanf devuelve cuántos datos leyó correctamente.
int leidos = scanf("%f %f %f %f %f %f %f %f %f %f %f %f %f", 
      &Goals, &Shots, &ShoPk, &PasTotCmp, &Assists, &CK, 
      &PassBlocks, &ScaDrib, &Int, &CrdY, &CrdR, &Off, &Fls);

if (leidos != 13) {
    printf("¡Error! No has introducido los 13 números correctamente.\n");
    return 1; // O manejar el error como prefieras
}

// Aquí mantienes tu lógica de si es -1, usar la media
if (Goals == -1) Goals = meanGoals;
if (Shots == -1) Shots = meanShots;
// ... etc
```
3) ** Validación de Lógica (Tiros < Goles) **
```
int datosValidos = 1; // Bandera para saber si todo está bien

// REGLA 1: No puedes meter goles sin tirar
if (Shots < Goals) {
    printf("ERROR LÓGICO: El jugador ha metido %0.f goles pero solo hizo %0.f tiros.\n", Goals, Shots);
    datosValidos = 0;
}

// REGLA 2: Tiros a puerta no pueden ser mayores que tiros totales
// (ShoPk suele ser Shots on Target/Penalty Kicks, asumo que es Shots on Target aquí)
if (ShoPk > Shots) {
    printf("ERROR LÓGICO: Tienes más tiros a puerta (%0.f) que tiros totales (%0.f).\n", ShoPk, Shots);
    datosValidos = 0;
}

// REGLA 3: No puede haber datos negativos (salvo el -1 que usamos de comodín)
if (PasTotCmp < 0 && PasTotCmp != -1) { // Ejemplo con pases
     printf("ERROR: No puedes tener pases negativos.\n");
     datosValidos = 0;
}

if (datosValidos == 0) {
    printf("Por favor, reinicia el programa e introduce datos coherentes.\n");
    return 1; // Salimos del programa
}
``
