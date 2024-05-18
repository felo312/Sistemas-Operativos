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

#ifndef BIBLIO02_H
#define BIBLIO02_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define SZ 6

struct timeval start, stop;

void inicio_tiempo();
void fin_tiempo();
void impresionMatriz(int M[SZ][SZ]);
int llenarMatriz(int M[SZ][SZ]);
void multMatriz(int A[SZ][SZ], int B[SZ][SZ], int result[SZ][SZ]);

#endif 
