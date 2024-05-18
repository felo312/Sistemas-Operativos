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
#include "biblio02.h"

void inicio_tiempo() {
    gettimeofday(&start, NULL);
}

void fin_tiempo() {
    gettimeofday(&stop, NULL);
    stop.tv_sec -= start.tv_sec;
    printf("%9.0f µs\n", (double)(stop.tv_sec * 1000000 + stop.tv_usec));
}

void impresionMatriz(int M[SZ][SZ]) {
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            printf(" %d ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int llenarMatriz(int M[SZ][SZ]) {
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            M[i][j] = 1 + rand() % 7;
        }
    }
}

void multMatriz(int A[SZ][SZ], int B[SZ][SZ], int result[SZ][SZ]) {
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            result[i][j] = 0;
            for (int k = 0; k < SZ; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

