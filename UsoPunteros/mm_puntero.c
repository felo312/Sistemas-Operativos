/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Fecha: 25 febrero 2024
Materia: Sistemas Operativos
Tema: Reserva de memoria y trabajo con punteros
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include "puntero.h"
#include <sys/time.h>
#include <time.h>

// Se reserva un espacio de memeria gigante:
#define DataSize (1024 * 1024 * 64 * 8)

// Se crea el espacio de memoria con tipo double:
static double MEM_CHUNK[DataSize];

int main(int argc, char *argv[]) {

  int SZ, i, j, k;
  if (argc > 1) {
    SZ = (int)atof(argv[1]);
  } else {
    printf("Entrada de argumentos inválidos\n");
    printf("$./ejecutable SizeMatriz\n");
    return -1;
  }
  printf("Matriz [%d][%d] \n", SZ, SZ);

  // Declarar matrices (Solo apuntadores):
  double *Ma, *Mb, *Mc;

  // Apuntar al espacio memoria de tamaño SZ*SZ:
  Ma = MEM_CHUNK;
  Mb = Ma + SZ * SZ;
  Mc = Mb + SZ * SZ;

  //Llamar función para llenar matriz:
  Matriz_Init(SZ, Ma, Mb, Mc);

  // Imprimir matriz:
  printf("Matriz 1:\n");
  ImpMatriz(SZ, Ma);

  printf("\nMatriz 2:\n");
  ImpMatriz(SZ, Mb);

//Multiplicar las matrices y mostrarlas:
  MultMat(SZ, Ma, Mb, Mc);
  printf("\nMatriz resultado:\n");
  ImpMatriz(SZ, Mc);

  return 0;
}

//gcc mm_puntero.c puntero.c -o ex2
