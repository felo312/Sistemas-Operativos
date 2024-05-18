/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Fecha: 02 abril 2024
Materia: Sistemas Operativos
Tema: Procesamiento paralelo
Objetivo: Crear una aplicación que use el algoritmo de multiplicación de matrices clásica (filas x columnas). La aplicación presentará diversos pasos como metodología para la implementación de un algoritmo. La implementación se hará para matrices cuadradas, es decir, el número de filas es igual a las columnas.

A continuación se presentan las diferentes etapas:
    -Reserva de memoria
    -Se crea el main para ingreso de argumentos de entrada
      -Arg1 dimensión matriz (NxN)
      -Arg2 cantidad de hilos
    -Validar argumentos de entrada
    -Crear punteros para las matrices
    -Inicializar matrices
    -Impresión de matrices
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define RESERVA (1024*128*64*8)
static double MEM_CHUNK[RESERVA];


int main(int argc, char *argv[]){

  if(argc <=2){
    printf("\nArgumentos de entrada \n");
    printf("\n\t $./ejecutable.exe Dim Hilos \n");
    return -1;
  }

  int N = (int) atof(argv[1]);
  int Th = (int) atof(argv[2]);
  
  if(N<=0){
    printf("\nDimensión matiz incorrecta \n");
    printf("\n\t Debe ser mayor que cero \n");
    return -1;
  }

  double *mA, *mB, *mC;

//Memoria continua apuntadores de la matriz 
  mA = MEM_CHUNK;
  mB = mA + N*N;
  mC = mB + N*N;

//Llenar matriz
  for(int i=0; i<N*N; i++){
    mA[i] = i*1.1;
    mB[i] = i*2.2;
    mC[i] = i;
  }

  for( int i=0; i<N*N; i++){
    if(i%N==0) printf("\n");
//  mA[i] =i*1.1;
    printf(" %f ", mA[i]);
  }

  printf("\n\n Fin del programa ");
  return 0;
}

//gcc mm_v0.c -o exe_mm