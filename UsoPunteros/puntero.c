/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Fecha: 25 febrero 2024
Materia: Sistemas Operativos
Tema: Reserva de memoria y trabajo con punteros
*************************************/

#include "puntero.h"
#include <stdio.h>
#include <stdlib.h>

// Función inicialización para llenar matrices:
void Matriz_Init(int T, double *a, double *b, double *c) {
  for (int i = 0; i < T * T; i++) {
    a[i] = 2.0 + i;
    b[i] = 1.0 + i;
    c[i] = 0.0;
  }
}

//Función para imprimir matrices:
void ImpMatriz(int SZ, double *mat) {
  for (int i = 0; i <SZ; i++) {
    for (int j = 0; j <SZ; j++) {
      printf(" %g ", mat[i +j * SZ]); //" %g "
    }
    printf("\n");
  }
}

// Función para multiplicar matrices:
void MultMat(int SZ, double *a, double *b, double *c) {
  for (int i = 0; i < SZ; i++) {
    for (int j = 0; j < SZ; j++) {
      for (int k = 0; k < SZ; k++) {
        c[i +j * SZ]+= a[i + k *SZ] * b[k+j*SZ];
      }
    }
  }
}

