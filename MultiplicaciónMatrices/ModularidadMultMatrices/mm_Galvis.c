/***** Captura de Tiempos ***********
Autor: Juan Felipe Galvis Vargas
Fecha: 6 febrero 2024
Materia: Sistemas Operativos
Pontificia Universidad Javeriana
Tema: Captura de tiempo de algoritmosgcc
                                - Multiplicación de Matrices
                                - Creación de Makefile (compilación
automatizada)
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "biblio02.h"

int main() {
    srand(time(NULL));

    int matriz01[SZ][SZ];
    int matriz02[SZ][SZ];
    int resultado[SZ][SZ];

    llenarMatriz(matriz01);
    llenarMatriz(matriz02);

    printf("Matriz 1:\n");
    impresionMatriz(matriz01);
    printf("Matriz 2:\n");
    impresionMatriz(matriz02);

    multMatriz(matriz01, matriz02, resultado);

    printf("Resultado de la multiplicación:\n");
    impresionMatriz(resultado);

    return 0;
}
