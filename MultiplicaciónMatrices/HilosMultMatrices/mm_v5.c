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
    -Funciones prog mod
        -inicializar
        -imprimir (solo si N<10)
    -Algoritmo clásico impresión matrices
    -Función para multiplicar matrices
    -Estructura de datos que encapsule datos
        -N, Th, Matrices
        -Se crea variable tipo struct
        -Se asigna valor
*************************************/

//librerías requeridas para la ejecución del código
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

//reserva de memoria 
#define RESERVA (1024*128*64*8)

//Estructura de datos que contiene la información de la matriz e hilos
struct datos_MM{
  int N, Th;
  double *mA, *mB, *mC;
};

static double MEM_CHUNK[RESERVA];

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
void multi_matrices(void *argMM){
  struct datos_MM *val = (struct datos_MM *)argMM;
  int N = val->N; //Se cambia el valor de N al valor desencapsulado
  
  for(int i=0; i<N; i++){ //filas
    for(int j=0; j<N; j++){ //columnas
      double SumTemp, *pA, *pB; //Variables para guardar los datos de las matrices
      SumTemp = 0.0; //Funciona como auxiliar para la suma
      pA= val->mA+ i * N;  
      pB= val->mB + j;
      for(int k=0; k<N; k++, pA++, pB+=N){ //productos
        SumTemp += *pA * *pB; //Multiplicación de los apuntadores de las matrices
      }
       val->mC[i*N+j] = SumTemp; //El resultado se asigna a la matriz resultado (mC)
    }  
  }
}

//función principal
int main(int argc, char *argv[]){

  //Validación de argumentos
  if(argc <=2){
    printf("\nArgumentos de entrada \n");
    printf("\n\t $./ejecutable.exe Dim Hilos \n");
    return -1;
  }
  //Casteo a enteros de los argumentos
  int N = (int) atof(argv[1]);
  int Th = (int) atof(argv[2]);

  //Validación matriz
  if(N<=0){
    printf("\nDimensión matriz incorrecta \n");
    printf("\n\t Debe ser mayor que cero \n");
    return -1;
  }

//Declaración apuntadores de las matrices  
  double *mA, *mB, *mC;
//Memoria continua apuntadores de la matriz 
  mA = MEM_CHUNK;
  mB = mA + N*N;
  mC = mB + N*N;

//Llamado a las funciones  
  init_matrices(N, mA, mB, mC);
  impr_matrices(N, mA);
  impr_matrices(N, mB);

//Reserva dinámica de datos_MM  
  struct datos_MM *encValores = (struct datos_MM *) malloc(sizeof(struct datos_MM));
  
  //Asignación de datos_MM
  encValores->N = N; //tamaño
  encValores->mA = mA; //matriz A
  encValores->mB = mB; //matriz B
  encValores->mC = mC; //matriz resultado
  
  multi_matrices(encValores);
  

  //Impresión matriz resultado haciendo uso del desencapsulamiento
  printf("\n-------------------------------\n");
  impr_matrices(N, encValores->mC);

  //fin:)
  printf("\n\nFin del programa\n");
  return 0;
}
//gcc mm_v5.c -o exe

