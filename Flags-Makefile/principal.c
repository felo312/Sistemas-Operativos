/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Co-autor: Andrés David Pérez
Fecha: 30 abril 2024
Materia: Sistemas Operativos
Tema: Flags- Makfile
*************************************/

//Bibliotecas necesarias para el funcionamiento del programa
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "biblio.h"

int main(int argc, char *argv[]) {
  
  // Se verifica que ingresen los argumentos necesarios de entrada
  if (argc != 11) {
    printf("\nArgumentos de entrada:\n\t%s -k edad -z genero -d estatura -s "
           "peso -f ruta_fichero\n\n",
           argv[0]);
    exit(EXIT_FAILURE); //salida
  }
  // reservar memoria para un puntero a la estructura
  struct datosEntrada *datos = (struct datosEntrada *)malloc(sizeof(struct datosEntrada));

  // Se llama a la función para obtener la información que el usuario ingresa por consola
  obtFlag(argc, argv, datos);

  //Apuntador de archivo tipo file (para usar el archivo)
  FILE *entrada;
  // Se crea una variable para calcular el tamaño de cada columna
  int tam = 0;
  // Se declaran las variables para calcular los promedios
  float entrada1 = 0.0, entrada2 = 0.0, promedio1 = 0.0, promedio2 = 0.0;
  // Se crea el canal para leer del fichero
  entrada = fopen(datos->fichero, "r");
  if (entrada == NULL) { // Si no encuentra la ruta arroja un error
    fputs("File error", stderr);
    exit(1);
  }

  printf("\nEl contenido del archivo de prueba es \n\n");
 // Lee cada línea del archivo
  while (fscanf(entrada, "%f", &entrada1) == 1 &&
         fscanf(entrada, "%f", &entrada2) == 1) {
    printf("%.0f, %.0f\n", entrada1, entrada2); // Se imprime las columnas
    // Se acumula los datos en cada variable dependiendo su columna
    promedio1 += entrada1;
    promedio2 += entrada2;
    
    tam ++;
  } 

  promedio1 = promedio1 / tam; //Se hace el cálculo del promedio dependiendo el tamaño de columna 1
  promedio2 = promedio2 / tam; //Se hace el cálculo del promedio dependiendo el tamaño de columna 2

  // Se imprime el promedio de datos de ambas columnas
  printf("\nEl promedio de los datos de la columna 1 es: %f\n", promedio1);
  printf("El promedio de los datos de la columna 2 es: %f", promedio2);

  // Se cierra el archivo de entrada de datos
  fclose(entrada);

  printf("\n\n\tFin del programa UwU\n");

  return 0;
}

