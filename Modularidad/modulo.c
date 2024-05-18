/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Fecha: 16 abril 2024
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
    -Funciones prog mod
        -inicializar
        -imprimir (solo si N<10)
    -Algoritmo clásico impresión matrices
    -Función para multiplicar matrices
    -Estructura de datos que encapsule datos
        -N, Th, Matrices
        -Se crea variable tipo struct
        -Se asigna valor
        -Se envían los valores a la función mult mat
    -Se crea vector de hilos pthreads que requiera el usuario
    -Se hace el envío de trabajo para cada hilo 
*************************************/

//librerías requeridas para la ejecución del código
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "modulo.h"

//Apuntadores de matrices
double *mA, *mB, *mC;

//Función para inicializar matrices
void init_matrices(int N, double *m1,double *m2,double *m3){
  for(int i=0; i<N*N; i++){ //matriz cuadrada N*N
    m1[i] = i*1.1;
    m2[i] = i*2.2;
    m3[i] = i;
  }
}

//Función para imprimir matrices
void impr_matrices(int N, double *matriz){
  if(N<10){ //verificación argumento del tamaño menor a 10
    for( int i=0; i<N*N; i++){
      if(i%N==0) printf("\n"); //Cada fila
      printf(" %f ", matriz[i]);
    }
    printf("\n-------------------------------\n");
  }
  else{
    printf("\n-------------------------------\n");
  }
}

//Función multiplicar matrices desencapsulando el argumento vacío
void *multi_matrices(void *argMM){
  struct datos_MM *val = (struct datos_MM *)argMM; 
  
  int hilo = val->IDHilo; //id del hilo
  int N = val->N; //Se cambia el valor de N (tamaño matrices) al valor desencapsulado
  int NumH = val->Th; // número de hilos 

 //Declaración del inicio y final del hilo en el que se divide la matriz:
  // Las filas se dividen por los hilos según el tamaño de la matriz
  int ini =hilo * (N /NumH); 
  //Se calcula el fin para el hilo obteniendo el siguiente hilo (+1)
  int fin =(hilo + 1) * (N /NumH); 
  
 //Se verifica si es el último hilo 
  if (hilo == NumH - 1) { //-1 por si pasa el límite del hilo
    fin = N; //el fin será tamaño N de las matrices cuadradas
  }
  
  for(int i= ini; i < fin; i++){ //hilo
    for(int j=0; j<N; j++){ //columnas
      double SumTemp, *pA, *pB; //Variables para guardar los datos de las matrices
      SumTemp = 0.0; //Funciona como auxiliar para la suma
      pA= val->mA+ i * N; //Fila i, tamaño N
      pB= val->mB + j; //inicio de las columnas j
      for(int k=0; k<N; k++, pA++, pB+=N){ //productos
        SumTemp += *pA * *pB; //Multiplicación de los apuntadores de las matrices
      }
       val->mC[i*N+j] = SumTemp; //El resultado se asigna a la matriz resultado (mC)
    }  
  }
  pthread_exit(NULL);
}