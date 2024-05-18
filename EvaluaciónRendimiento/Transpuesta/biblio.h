/**************************************************************
	Pontificia Universidad Javeriana
	Autor: Juan Felipe Galvis, Andrés David Pérez Cely, Juan Diego Reyes
	Fecha: 07 mayo 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/
#ifndef __BIBLIO_H__
#define __BIBLIO_H__

#include <pthread.h>


struct parametros {
	int nH;  // Número total de hilos
	int idH; // Identificador del hilo actual
	int N;   // Tamaño de las matrices cuadradas NxN
};

/*Se hacen las siguientes definiciones con extern para que no haya conflicto
con la función principal.
*/
extern double *mA, *mB, *mC; // Punteros a las matrices A, B y C
extern struct timeval start, stop; // Variables para medir el tiempo de ejecución
extern pthread_mutex_t MM_mutex;// Definición del mutex para controlar acceso a la matriz


void llenar_matriz(int SZ);

void print_matrix(int sz, double *matriz);

void inicial_tiempo();

void final_tiempo();

void *mult_thread(void *variables);

#endif