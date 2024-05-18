#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h> 

struct Sensor {
  int tipo;
  int tiempo;
  char nombreArchivo[100];
  char nombrePipe[100];
};

//Asignación de los datos
void llenarDatos(struct Sensor *sensor, char instruccion[], char contenido[]);
//Lectura de los datos
void imprimirDatos(struct Sensor sensor);

int main(int argc, char *argv[]) {
  // Declaración de variables
  struct Sensor sensor;
  /*
  int sensor_type;
  int time_interval;
  char *file_name;
  char *pipe_name;
  */
  sensor.tipo = -1;
  sensor.tiempo = -1;
  sensor.nombreArchivo[0] = '\0';
  sensor.nombrePipe[0] = '\0';
  
  int pipe_fd;
  FILE *file_ptr;
  char buffer[100]; // Ajustar el tamaño según sea necesario
  buffer[0] = '\0';

  // Verificar el número correcto de argumentos
  if (argc != 9) {
      fprintf(stderr, 
        "\nArgumentos de entrada:\n\n\t %s -s tipo_sensor -t tiempo -f archivo -p pipe_nominal\n", argv[0]);
      exit(EXIT_FAILURE);
  }

  // Procesar los argumentos de la línea de comandos
  for(int i = 1; i < argc; i+=2){
    llenarDatos(&sensor, argv[i], argv[i+1]);
  }
  /*
  for (int i = 1; i < argc; i += 2) {
      if (strcmp(argv[i], "-s") == 0) {
          sensor_type = atoi(argv[i + 1]);
      } else if (strcmp(argv[i], "-t") == 0) {
          time_interval = atoi(argv[i + 1]);
      } else if (strcmp(argv[i], "-f") == 0) {
          file_name = argv[i + 1];
      } else if (strcmp(argv[i], "-p") == 0) {
          pipe_name = argv[i + 1];
      } else {
          fprintf(stderr, "Argumento desconocido: %s\n", argv[i]);
          exit(EXIT_FAILURE);
      }
  }
  */

  // Abrir el archivo de datos
  file_ptr = fopen(sensor.nombreArchivo, "r");
  if (file_ptr == NULL) {
      perror("Error al abrir el archivo");
      exit(EXIT_FAILURE);
  }

  // Abrir el pipe nominal
  mkfifo(sensor.nombrePipe, 0666);
  pipe_fd = open(sensor.nombrePipe, O_WRONLY);
  if (pipe_fd == -1) {
      perror("Error al abrir el pipe nominal");
      exit(EXIT_FAILURE);
  }
  
  
  // Leer datos del archivo y enviar al monitor a través del pipe
  while (fgets(buffer, sizeof(buffer), file_ptr) != NULL) {
    if(sensor.tipo == 1){
      strcat(buffer, "_1");
    }
    if(sensor.tipo == 2){
      strcat(buffer, "_2");
    }
    // Enviar la medición al monitor
    if (write(pipe_fd, buffer, strlen(buffer) + 1) == -1) {
        perror("Error al escribir en el pipe");
        exit(EXIT_FAILURE);
    }

    // Esperar el intervalo de tiempo especificado
    sleep(sensor.tiempo);
  }

  // Cerrar el archivo y el pipe
  fclose(file_ptr);
  close(pipe_fd);

  return 0;
}

//Asignación de los datos
void llenarDatos(struct Sensor *sensor, char instruccion[], char contenido[]){
  if(strcmp(instruccion, "-s") == 0){
    sensor->tipo = atoi(contenido);
  }  
  if(strcmp(instruccion, "-t") == 0){
    sensor->tiempo = atoi(contenido);
  }
  if(strcmp(instruccion, "-f") == 0){
    strcpy(sensor->nombreArchivo, contenido);
  }
  if(strcmp(instruccion, "-p") == 0){
    strcpy(sensor->nombrePipe, contenido);
  }
}
//Lectura de los datos
void imprimirDatos(struct Sensor sensor){
  printf("Tipo: %d\n", sensor.tipo);
  printf("Tiempo: %d\n", sensor.tiempo);
  printf("Nombre del archivo: %s\n", sensor.nombreArchivo);
  printf("Nombre del pipe: %s\n", sensor.nombrePipe);
}

/*
tar -cvf proyecto_pre.tar monitor.c sensor.c ph.txt temperatura.txt
gzip proyecto_pre.tar
*/
