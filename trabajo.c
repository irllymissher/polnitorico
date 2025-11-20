#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "colaEnteros.h"
#include "minMonticulo.h"

#define MAX_COLUMNS 150


// Función para obtener el número correspondiente a cada posición
int obtenerNumeroPosicion(char *posicion) {
    char *posiciones[] = {"DF", "GK", "FW", "MF"};
    int numeros[] = {1, 2, 3, 4};
    while (*posicion == ' ' || *posicion == '\t') { // Eliminar espacios iniciales
        posicion++;
    }
    for (int i = 0; i < 4; i++) {
        if (strcmp(posicion, posiciones[i]) == 0) {
            return numeros[i];
        }
    }
    return 0;
}

float normalizar(float valor, float minimo, float maximo) {
    if ((maximo - minimo) != 0) {
        return (valor - minimo) / (maximo - minimo);
    } else {
        return valor;
    }
}

int main()
{

    FILE *file = fopen("2022-2023 Football Player Stats.csv", "r");

    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return 1;
    }

    char line[1024];
    tipoCola ColaGoals,ColaShots,ColaShoPk,ColaPasTotCmp,ColaAssists,ColaCK,ColaPasBlocks,ColaScaDrib,ColaInt,ColaCrdY,ColaCrdR,ColaOff,ColaFls;
	tipoCola ColaGoals2,ColaShots2,ColaShoPk2,ColaPasTotCmp2,ColaAssists2,ColaCK2,ColaPasBlocks2,ColaScaDrib2,ColaInt2,ColaCrdY2,ColaCrdR2,ColaOff2,ColaFls2;
	tipoCola ColaPos;
    tipoColaDeColas cola;
	tipoMinMonticulo monticulo;
	nuevoMinMonticulo(&monticulo, 988);

    nuevaCola(&ColaGoals);
	nuevaCola(&ColaShots);
	nuevaCola(&ColaShoPk);
    nuevaCola(&ColaPasTotCmp);
    nuevaCola(&ColaAssists);
	nuevaCola(&ColaCK);
	nuevaCola(&ColaPasBlocks);
	nuevaCola(&ColaScaDrib);
	nuevaCola(&ColaInt);
    nuevaCola(&ColaCrdY);
    nuevaCola(&ColaCrdR);
	nuevaCola(&ColaOff);
    nuevaCola(&ColaFls);
    nuevaCola(&ColaPos);
	nuevaCola(&ColaGoals2);
	nuevaCola(&ColaShots2);
	nuevaCola(&ColaShoPk2);
    nuevaCola(&ColaPasTotCmp2);
    nuevaCola(&ColaAssists2);
	nuevaCola(&ColaCK2);
	nuevaCola(&ColaPasBlocks2);
	nuevaCola(&ColaScaDrib2);
	nuevaCola(&ColaInt2);
    nuevaCola(&ColaCrdY2);
    nuevaCola(&ColaCrdR2);
	nuevaCola(&ColaOff2);
    nuevaCola(&ColaFls2);

    // Acumuladores para calcular medias y ofrecer sugerencias al usuario
    float sumGoals=0, sumShots=0, sumShoPk=0, sumPasTotCmp=0, sumAssists=0, sumCK=0, sumPasBlocks=0, sumScaDrib=0, sumInt=0, sumCrdY=0, sumCrdR=0, sumOff=0, sumFls=0;
    int countGoals=0, countShots=0, countShoPk=0, countPasTotCmp=0, countAssists=0, countCK=0, countPasBlocks=0, countScaDrib=0, countInt=0, countCrdY=0, countCrdR=0, countOff=0, countFls=0;

	int lineNumber = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {//Leemos las lineas del csv y extraemos datos de cada columna 
		lineNumber++;
		if (lineNumber == 1) {
			continue;  // Saltar la primera línea
		}
		char *token;
        char *columnas[MAX_COLUMNS];
        int cont = 0;

		// Tokenizar la línea separada por ";"
        token = strtok(line, ";");
        while (token != NULL) {
            columnas[cont] = token;
            cont++;
            token = strtok(NULL, ";");
        }
		/*printf("Línea leída: %s\n", line);
		printf("Columnas tokenizadas:\n");
		for (int i = 0; i < cont; i++) {
			printf("Columna %d: %s\n", i, columnas[i]);
		}*/
		// Procesar posición (columna 3)
        if (cont >= 4)
		{
			char *posicion = columnas[3];//Extraemos la posicion del jugador.
			int numeroPosicion = obtenerNumeroPosicion(posicion);
			if (numeroPosicion != 0) {
				encolar(&ColaPos, (float)numeroPosicion);
			}
		}
		//Procesar datos
        if (cont >= 120) {
            encolar(&ColaGoals, atoi(columnas[12]));
			encolar(&ColaShots, atoi(columnas[13]));
			encolar(&ColaShoPk, atoi(columnas[20]));
			encolar(&ColaPasTotCmp,atoi(columnas[22]));
			encolar(&ColaAssists, atoi(columnas[36]));
			encolar(&ColaCK, atoi(columnas[50]));
			encolar(&ColaPasBlocks, atoi(columnas[56]));
			encolar(&ColaScaDrib, atoi(columnas[60]));
			encolar(&ColaInt, atoi(columnas[83]));
			encolar(&ColaCrdY,atoi(columnas[109]));
			encolar(&ColaCrdR,atoi(columnas[110]));
			encolar(&ColaOff, atoi(columnas[114]));
			encolar(&ColaFls, atoi(columnas[112]));
        }
    }

    while (!esNulaCola(ColaGoals)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaGoals)) {
			aux = frente(ColaGoals);
			desencolar(&ColaGoals);
			// acumular para media
			sumGoals += aux;
			countGoals++;
			if ((ColaGoals.maximo - ColaGoals.minimo) != 0) { 
				aux = normalizar(aux,ColaGoals.minimo,ColaGoals.maximo);
				encolar(&ColaGoals2, aux);
			} else {
			}
		}
	}

	while (!esNulaCola(ColaShots)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaShots)) {
			aux = frente(ColaShots);
			desencolar(&ColaShots);
			sumShots += aux;
			countShots++;
			if ((ColaShots.maximo - ColaShots.minimo) != 0) {
				aux = normalizar(aux,ColaShots.minimo,ColaShots.maximo);
				encolar(&ColaShots2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaShoPk)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaShoPk)) {
			aux = frente(ColaShoPk);
			desencolar(&ColaShoPk);
			// acumular para media
			sumShoPk += aux;
			countShoPk++;
			if ((ColaShoPk.maximo - ColaShoPk.minimo) != 0) {
				aux = normalizar(aux,ColaShoPk.minimo,ColaShoPk.maximo);
				encolar(&ColaShoPk2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaPasTotCmp)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaPasTotCmp)) {
			aux = frente(ColaPasTotCmp);
			desencolar(&ColaPasTotCmp);
			sumPasTotCmp += aux;
			countPasTotCmp++;
			if ((ColaPasTotCmp.maximo - ColaPasTotCmp.minimo) != 0) {
				aux = normalizar(aux,ColaPasTotCmp.minimo,ColaPasTotCmp.maximo);
				encolar(&ColaPasTotCmp2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaAssists)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaAssists)) {
			aux = frente(ColaAssists);
			desencolar(&ColaAssists);
			sumAssists += aux;
			countAssists++;
			if ((ColaAssists.maximo - ColaAssists.minimo) != 0) {
				aux = normalizar(aux,ColaAssists.minimo,ColaAssists.maximo);
				encolar(&ColaAssists2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaCK)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaCK)) {
			aux = frente(ColaCK);
			desencolar(&ColaCK);
			sumCK += aux;
			countCK++;
			if ((ColaCK.maximo - ColaCK.minimo) != 0) {
				aux = normalizar(aux,ColaCK.minimo,ColaCK.maximo);
				encolar(&ColaCK2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaPasBlocks)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaPasBlocks)) {
			aux = frente(ColaPasBlocks);
			desencolar(&ColaPasBlocks);
			sumPasBlocks += aux;
			countPasBlocks++;
			if ((ColaPasBlocks.maximo - ColaPasBlocks.minimo) != 0) {
				aux = normalizar(aux,ColaPasBlocks.minimo,ColaPasBlocks.maximo);
				encolar(&ColaPasBlocks2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaScaDrib)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaScaDrib)) {
			aux = frente(ColaScaDrib);
			desencolar(&ColaScaDrib);
			sumScaDrib += aux;
			countScaDrib++;
			if ((ColaScaDrib.maximo - ColaScaDrib.minimo) != 0) {
				aux = normalizar(aux,ColaScaDrib.minimo,ColaScaDrib.maximo);
				encolar(&ColaScaDrib2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaInt)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaInt)) {
			aux = frente(ColaInt);
			desencolar(&ColaInt);
			sumInt += aux;
			countInt++;
			if ((ColaInt.maximo - ColaInt.minimo) != 0) {
				aux = normalizar(aux,ColaInt.minimo,ColaInt.maximo);
				encolar(&ColaInt2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaCrdY)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaCrdY)) {
			aux = frente(ColaCrdY);
			desencolar(&ColaCrdY);
			sumCrdY += aux;
			countCrdY++;
			if ((ColaCrdY.maximo - ColaCrdY.minimo) != 0) {
				aux = normalizar(aux,ColaCrdY.minimo,ColaCrdY.maximo);
				encolar(&ColaCrdY2, aux);
			} else {
			}
		}
	}

	while (!esNulaCola(ColaCrdR)) {
		tipoElementoCola aux;
		if (!esNulaCola(ColaCrdR)) {
			aux = frente(ColaCrdR);
			desencolar(&ColaCrdR);
			sumCrdR += aux;
			countCrdR++;
			if ((ColaCrdR.maximo - ColaCrdR.minimo) != 0) {
				aux = normalizar(aux,ColaCrdR.minimo,ColaCrdR.maximo);
				encolar(&ColaCrdR2, aux);
			} else {
			}
		}
	}

	while (!esNulaCola(ColaOff)){
		tipoElementoCola aux;
		if (!esNulaCola(ColaOff)) {
			aux = frente(ColaOff);
			desencolar(&ColaOff);
			sumOff += aux;
			countOff++;
			if ((ColaOff.maximo - ColaOff.minimo) != 0) {
				aux = normalizar(aux,ColaOff.minimo,ColaOff.maximo);
				encolar(&ColaOff2, aux);
			} else {
			}
		}
	}
	while (!esNulaCola(ColaFls)){
		tipoElementoCola aux;
		if (!esNulaCola(ColaFls)) {
			aux = frente(ColaFls);
			desencolar(&ColaFls);
			sumFls += aux;
			countFls++;
			if ((ColaFls.maximo - ColaFls.minimo) != 0) {
				aux = normalizar(aux,ColaFls.minimo,ColaFls.maximo);
				encolar(&ColaFls2, aux);
			} else {
			}
		}
	}
	

	//GUARDAR DATOS EN COLA DE COLAS
	nuevaColaDeColas(&cola);
	encolarCola(&cola, ColaGoals2);
    encolarCola(&cola, ColaShots2);
    encolarCola(&cola, ColaShoPk2);
    encolarCola(&cola, ColaPasTotCmp2);
    encolarCola(&cola, ColaAssists2);
    encolarCola(&cola, ColaCK2);
    encolarCola(&cola, ColaPasBlocks2);
    encolarCola(&cola, ColaScaDrib2);
    encolarCola(&cola, ColaInt2);
	encolarCola(&cola, ColaCrdY2);
	encolarCola(&cola, ColaCrdR2);
	encolarCola(&cola, ColaOff2);
	encolarCola(&cola, ColaFls2);

	//Pedimos datos.
	//DISTANCIA
	float Goals,Shots,ShoPk,PasTotCmp,Assists,CK,PassBlocks,ScaDrib, Int,CrdY,CrdR,Off,Fls,sumatorio,distancia;
	sumatorio = 0;
	distancia = 0;

	// Calcular medias (si count == 0 -> media 0)
	float meanGoals = (countGoals>0) ? (sumGoals / countGoals) : 0;
	float meanShots = (countShots>0) ? (sumShots / countShots) : 0;
	float meanShoPk = (countShoPk>0) ? (sumShoPk / countShoPk) : 0;
	float meanPasTotCmp = (countPasTotCmp>0) ? (sumPasTotCmp / countPasTotCmp) : 0;
	float meanAssists = (countAssists>0) ? (sumAssists / countAssists) : 0;
	float meanCK = (countCK>0) ? (sumCK / countCK) : 0;
	float meanPassBlocks = (countPasBlocks>0) ? (sumPasBlocks / countPasBlocks) : 0;
	float meanScaDrib = (countScaDrib>0) ? (sumScaDrib / countScaDrib) : 0;
	float meanInt = (countInt>0) ? (sumInt / countInt) : 0;
	float meanCrdY = (countCrdY>0) ? (sumCrdY / countCrdY) : 0;
	float meanCrdR = (countCrdR>0) ? (sumCrdR / countCrdR) : 0;
	float meanOff = (countOff>0) ? (sumOff / countOff) : 0;
	float meanFls = (countFls>0) ? (sumFls / countFls) : 0;

	printf("Introduce los valores del jugador a predecir. Si no los conoces, introduce -1 para usar la media sugerida.\n");

	printf("Goals (min=%.2f, max=%.2f, mean=%.2f): ", ColaGoals.minimo, ColaGoals.maximo, meanGoals);
	scanf("%f", &Goals);
	if (Goals == -1) Goals = meanGoals;

	printf("Shots (min=%.2f, max=%.2f, mean=%.2f): ", ColaShots.minimo, ColaShots.maximo, meanShots);
	scanf("%f", &Shots);
	if (Shots == -1) Shots = meanShots;

	printf("ShoPk (min=%.2f, max=%.2f, mean=%.2f): ", ColaShoPk.minimo, ColaShoPk.maximo, meanShoPk);
	scanf("%f", &ShoPk);
	if (ShoPk == -1) ShoPk = meanShoPk;

	printf("PasTotCmp (min=%.2f, max=%.2f, mean=%.2f): ", ColaPasTotCmp.minimo, ColaPasTotCmp.maximo, meanPasTotCmp);
	scanf("%f", &PasTotCmp);
	if (PasTotCmp == -1) PasTotCmp = meanPasTotCmp;

	printf("Assists (min=%.2f, max=%.2f, mean=%.2f): ", ColaAssists.minimo, ColaAssists.maximo, meanAssists);
	scanf("%f", &Assists);
	if (Assists == -1) Assists = meanAssists;

	printf("CK (min=%.2f, max=%.2f, mean=%.2f): ", ColaCK.minimo, ColaCK.maximo, meanCK);
	scanf("%f", &CK);
	if (CK == -1) CK = meanCK;

	printf("PassBlocks (min=%.2f, max=%.2f, mean=%.2f): ", ColaPasBlocks.minimo, ColaPasBlocks.maximo, meanPassBlocks);
	scanf("%f", &PassBlocks);
	if (PassBlocks == -1) PassBlocks = meanPassBlocks;

	printf("ScaDrib (min=%.2f, max=%.2f, mean=%.2f): ", ColaScaDrib.minimo, ColaScaDrib.maximo, meanScaDrib);
	scanf("%f", &ScaDrib);
	if (ScaDrib == -1) ScaDrib = meanScaDrib;

	printf("Int (min=%.2f, max=%.2f, mean=%.2f): ", ColaInt.minimo, ColaInt.maximo, meanInt);
	scanf("%f", &Int);
	if (Int == -1) Int = meanInt;

	printf("CrdY (min=%.2f, max=%.2f, mean=%.2f): ", ColaCrdY.minimo, ColaCrdY.maximo, meanCrdY);
	scanf("%f", &CrdY);
	if (CrdY == -1) CrdY = meanCrdY;

	printf("CrdR (min=%.2f, max=%.2f, mean=%.2f): ", ColaCrdR.minimo, ColaCrdR.maximo, meanCrdR);
	scanf("%f", &CrdR);
	if (CrdR == -1) CrdR = meanCrdR;

	printf("Off (min=%.2f, max=%.2f, mean=%.2f): ", ColaOff.minimo, ColaOff.maximo, meanOff);
	scanf("%f", &Off);
	if (Off == -1) Off = meanOff;

	printf("Fls (min=%.2f, max=%.2f, mean=%.2f): ", ColaFls.minimo, ColaFls.maximo, meanFls);
	scanf("%f", &Fls);
	if (Fls == -1) Fls = meanFls;
	/*
	Goals = 10;
	Shots = 0;
	ShoPk = 53.3;
	PasTotCmp = 0.57;
	Assists = 0.57;
	CK = 2.46;
	PassBlocks = 1.83;
	ScaDrib = 0.57;
	Int = 0;
	CrdY = 0;
	CrdR = 0;
	Off = 0.29;
	Fls = 0.17;
	*/

	
	tipoElementoMinMonticulo res;

	while (!esNulaColaDeColas(cola)) {
		if (esNulaCola(ColaShots2)||esNulaCola(ColaPos)||esNulaCola(ColaShoPk2))
		{
			break;
		}
		Goals = normalizar(Goals,ColaGoals.minimo,ColaGoals.maximo);
		sumatorio += (fabs(frente(ColaGoals2)-Goals)*fabs(frente(ColaGoals2)-Goals));
		desencolar(&ColaGoals2);

	
		Shots = normalizar(Shots,ColaShots.minimo,ColaShots.maximo);
		sumatorio += (fabs(frente(ColaShots2)-Shots)*fabs(frente(ColaShots2)-Shots));
		desencolar(&ColaShots2);
	
		ShoPk = normalizar(ShoPk,ColaShoPk.minimo,ColaShoPk.maximo);
		sumatorio += (fabs(frente(ColaShoPk2)-ShoPk)*fabs(frente(ColaShoPk2)-ShoPk));
		desencolar(&ColaShoPk2);

		PasTotCmp = normalizar(PasTotCmp,ColaPasTotCmp.minimo,ColaPasTotCmp.maximo);
		sumatorio += (fabs(frente(ColaPasTotCmp2)-PasTotCmp)*fabs(frente(ColaPasTotCmp2)-PasTotCmp));
		desencolar(&ColaPasTotCmp2);

		Assists = normalizar(Assists,ColaAssists.minimo,ColaAssists.maximo);
		sumatorio += fabs(frente(ColaAssists2)-Assists)*fabs(frente(ColaAssists2)-Assists);
		desencolar(&ColaAssists2);


		CK = normalizar(CK,ColaCK.minimo,ColaCK.maximo);
		sumatorio += fabs(frente(ColaCK2)-CK)*fabs(frente(ColaCK2)-CK);
		desencolar(&ColaCK2);

		PassBlocks = normalizar(PassBlocks,ColaPasBlocks.minimo,ColaPasBlocks.maximo);
		sumatorio += fabs(frente(ColaPasBlocks2)-PassBlocks)*fabs(frente(ColaPasBlocks2)-PassBlocks);
		desencolar(&ColaPasBlocks2);

		ScaDrib = normalizar(ScaDrib,ColaScaDrib.minimo,ColaScaDrib.maximo);
		sumatorio += fabs(frente(ColaScaDrib2)-ScaDrib)*fabs(frente(ColaScaDrib2)-ScaDrib);
		desencolar(&ColaScaDrib2);

		Int = normalizar(Int,ColaInt.minimo,ColaInt.maximo);
		sumatorio += fabs(frente(ColaInt2)-Int)*fabs(frente(ColaInt2)-Int);
		desencolar(&ColaInt2);

		CrdY = normalizar(CrdY,ColaCrdY.minimo,ColaCrdY.maximo);
		sumatorio += fabs(frente(ColaCrdY2)-CrdY)*fabs(frente(ColaCrdY2)-CrdY);
		desencolar(&ColaCrdY2);

		CrdR = normalizar(CrdR,ColaCrdR.minimo,ColaCrdR.maximo);
		sumatorio += fabs(frente(ColaCrdR2)-CrdR)*fabs(frente(ColaCrdR2)-CrdR);
		desencolar(&ColaCrdR2);

		Off = normalizar(Off,ColaOff.minimo,ColaOff.maximo);
		sumatorio += fabs(frente(ColaOff2)-Off)*fabs(frente(ColaOff2)-Off);
		desencolar(&ColaOff2);

		Fls = normalizar(Fls,ColaFls.minimo,ColaFls.maximo);
		sumatorio += fabs(frente(ColaFls2)-Fls)*fabs(frente(ColaFls2)-Fls);
		desencolar(&ColaFls2);
		distancia = sqrt(sumatorio);
		//printf("Distancia: %f\n", distancia);
		
		res.numero = distancia;
		res.posicion = frente(ColaPos);
		desencolar(&ColaPos);
		insertarMinMonticulo(&monticulo, res);
		sumatorio = 0;
		res.numero = 0;
	}

	// Pedimos K y realizamos votación K-NN usando los K elementos con menor distancia
	int K;
	printf("Introduce K (numero de vecinos a considerar, por ejemplo 1 o 3): ");
	scanf("%d", &K);
	if (K < 1) K = 1;

	int votes[5] = {0,0,0,0,0};
	float sumDist[5] = {0,0,0,0,0};

	for (int i = 0; i < K; i++) {
		if (esVacio(monticulo)) break;
		(tipoElementoMinMonticulo)res = extraerMin(&monticulo);
		int posv = res.posicion;
		if (posv >= 1 && posv <= 4) {
			votes[posv]++;
			sumDist[posv] += res.numero;
		}
	}

	// Elegir posición con más votos; en caso de empate, elegir la de menor distancia media
	int bestPos = -1;
	int bestVotes = -1;
	float bestAvgDist = 1e30;
	for (int p = 1; p <= 4; p++) {
		if (votes[p] > bestVotes) {
			bestVotes = votes[p];
			bestPos = p;
			bestAvgDist = (votes[p] > 0) ? (sumDist[p] / votes[p]) : 1e30;
		} else if (votes[p] == bestVotes && votes[p] > 0) {
			float avg = sumDist[p] / votes[p];
			if (avg < bestAvgDist) {
				bestPos = p;
				bestAvgDist = avg;
			}
		}
	}

	int posicion = bestPos;

	switch (posicion) {
		case 1:
			printf("La posición del jugador es DF\n");
			break;
		case 2:
			printf("La posición del jugador es GK\n");
			break;
		case 3:
			printf("La posición del jugador es FW\n");
			break;
		case 4:
			printf("La posición del jugador es MF\n");
			break;
		default:
			printf("No se ha podido determinar la posición del jugador\n");
			break;
	}

    fclose(file);

    return 0;
}
