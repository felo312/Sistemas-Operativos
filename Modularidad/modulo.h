#ifndef __MODULO_H__
#define __MODULO_H__

//Estructura de datos que contiene la información de la matriz e hilos
struct datos_MM{
  int N, Th, IDHilo;
  double *mA, *mB, *mC;
};

//Función para inicializar matrices
void init_matrices(int N, double *m1,double *m2,double *m3);

//Función para imprimir matrices
void impr_matrices(int N, double *matriz);

//Función multiplicar matrices desencapsulando el argumento vacío
void *multi_matrices(void *argMM);

#endif