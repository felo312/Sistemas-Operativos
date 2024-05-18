/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Co-autor: Andrés David Pérez
Fecha: 30 abril 2024
Materia: Sistemas Operativos
Tema: Principal Flags- Makfile
*************************************/

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Se requiere una estructura para almacenar los datos de entrada
struct datosEntrada {
  int edad, genero;
  float peso, estatura;
  char *fichero; // nombre del fichero a buscar
};

int main(int argc, char *argv[]) {

  if (argc != 11) {
    printf("\nArgumentos de entrada:\n\t%s -k edad -z genero -d estatura -s "
           "peso -f ruta_fichero\n\n",
           argv[0]);
    exit(EXIT_FAILURE);
  }
  // reservar memoria para un puntero a la estructura
  struct datosEntrada *datos =
      (struct datosEntrada *)malloc(sizeof(struct datosEntrada));
  // se requiere dadas unas banderas, reconocer el valor

  //$./ejecutable -k edad -z genero -d estatura -s peso

  // verificación de flags
  for (int i = 0; i < argc; i++) {
    if (i + 1 != argc) {
      if (strcmp(argv[i], "-k") == 0) {
        char *k = argv[i + 1];
        datos->edad = atoi(k);
        i++;
      }
      if (strcmp(argv[i], "-z") == 0) {
        char *z = argv[i + 1];
        datos->genero = atoi(z);
        i++;
      }
      if (strcmp(argv[i], "-d") == 0) {
        char *d = argv[i + 1];
        datos->estatura = atof(d);
        i++;
      }
      if (strcmp(argv[i], "-s") == 0) {
        char *s = argv[i + 1];
        datos->peso = atof(s);
        i++;
      }
      if (strcmp(argv[i], "-f") == 0) {
        char *f = argv[i + 1];
        datos->fichero = f;
        i++;
      }
    }
  }

  printf("./%s %s %d %s %d %s %f %s %f \n", argv[0], argv[1], datos->edad,
         argv[3], datos->genero, argv[5], datos->estatura, argv[7],
         datos->peso);

  FILE *entrada;
  int tam = 0;
  float entrada1 = 0.0, entrada2 = 0.0, promedio1 = 0.0, promedio2 = 0.0;
  entrada = fopen(datos->fichero, "r");
  if (entrada == NULL) {
    fputs("File error", stderr);
    exit(1);
  }

  printf("\nEl contenido del archivo de prueba es \n\n");
  while (fscanf(entrada, "%f", &entrada1) == 1 &&
         fscanf(entrada, "%f", &entrada2) == 1) {
    printf("%.0f, %.0f\n", entrada1, entrada2);
    promedio1 += entrada1;
    promedio2 += entrada2;

    tam += 2;
  }

  tam = tam / 2;

  promedio1 = promedio1 / tam;
  promedio2 = promedio2 / tam;

  printf("\nEl promedio de los datos de la columna 1 es: %f\n", promedio1);
  printf("El promedio de los datos de la columna 2 es: %f", promedio2);

  // Se cierra el archivo de entrada de datos
  fclose(entrada);

  printf("\nFin del programa UwU\n");

  return 0;
}

// gcc main.c -o exe
// ./exe -k 234 -z 43 -d 54 -s 53 -f doc.txt