/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Co-autor: Andrés David Pérez
Fecha: 30 abril 2024
Materia: Sistemas Operativos
Tema: Funciones
*************************************/

//Bibliotecas necesarias para el funcionamiento del programa
#include "biblio.h"
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Función para obtener las banderas
void obtFlag(int argc, char *argv[], struct datosEntrada *datos) {
  
  //Argumento k edad
  for (int i = 0; i < argc; i++) { //ciclo hasta la cantidad de argumentos
    if (i + 1 != argc) {  // +1 para que coincida la cantidad
      if (strcmp(argv[i], "-k") == 0) {
        char *k = argv[i + 1];
        datos->edad = atoi(k); //casteo edad de tipo entero
        i++;
      }
      if (strcmp(argv[i], "-z") == 0) {
        char *z = argv[i + 1];
        datos->genero = atoi(z); // casteo genero de tipo entero
        i++;
      }
      if (strcmp(argv[i], "-d") == 0) {
        char *d = argv[i + 1];
        datos->estatura = atof(d); // casteo edad de tipo float
        i++;
      }
      if (strcmp(argv[i], "-s") == 0) {
        char *s = argv[i + 1];
        datos->peso = atof(s); // casteo edad de tipo float
        i++;
      }
      if (strcmp(argv[i], "-f") == 0) {
        char *f = argv[i + 1];
        datos->fichero = f; //fichero de la estructura
        i++;
      }
    }
  }
}