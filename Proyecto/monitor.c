#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

/*
https://replit.com/join/utrkjfggcy-juanfelipe54415

*/
#define BUFFER_SIZE 10

struct Monitor{
  int tamanoBuffer;
  char archivoTemp[100];
  char archivoPH[100];
  char nombrePipe[100];
};

struct Sensor {
  int tipo;
  int tiempo;
  char nombreArchivo[100];
  char nombrePipe[100];
};

struct Buffer {
  float *temperatura;
  float *ph;
  int contTemp;
  int contPH;
};

// Estructura para los búferes de mediciones de temperatura y pH
typedef struct {
    float temperature[BUFFER_SIZE];
    float ph[BUFFER_SIZE];
    int temperature_count;
    int ph_count;
} Buffer;

// Variables globales
Buffer buffer;
sem_t empty_temperature;
sem_t full_temperature;
sem_t mutex_temperature;
sem_t empty_ph;
sem_t full_ph;
sem_t mutex_ph;

void llenarDatos(struct Monitor *monitor, char instruccion[], char contenido[]);

// Funciones para los hilos
void *recolector(void *arg);
void *ph_handler(void *arg);
void *temperature_handler(void *arg);

int main(int argc, char *argv[]) {
  struct Monitor monitor;
  monitor.tamanoBuffer = 0;
  monitor.archivoTemp[0] = '\0';
  monitor.archivoPH[0] = '\0';
  monitor.nombrePipe[0] = '\0';
  
  // Procesar argumentos de línea de comandos
  if (argc !=9) {
      fprintf(stderr, "Uso: %s -b tam_buffer -t file-temp -h file-ph -p pipe-nominal\n", argv[0]);
      exit(EXIT_FAILURE);
  }
  /*
  char *temperature_file_name;
  char *ph_file_name;
  char *pipe_name;
  int buffer_size;
  */
  for(int i =1; i < argc; i+=2){
    llenarDatos(&monitor, argv[i], argv[i+1]);
  }
  /*
  for (int i = 1; i < argc; i += 2) {
      if (strcmp(argv[i], "-b") == 0) {
          buffer_size = atoi(argv[i + 1]);
      } else if (strcmp(argv[i], "-t") == 0) {
          temperature_file_name = argv[i + 1];
      } else if (strcmp(argv[i], "-h") == 0) {
          ph_file_name = argv[i + 1];
      } else if (strcmp(argv[i], "-p") == 0) {
          pipe_name = argv[i + 1];
      } else {
          fprintf(stderr, "Argumento desconocido: %s\n", argv[i]);
          exit(EXIT_FAILURE);
      }
  }
*/

  // Inicializar semáforos
  sem_init(&empty_temperature, 0, monitor.tamanoBuffer);
  sem_init(&full_temperature, 0, 0);
  sem_init(&mutex_temperature, 0, 1);
  sem_init(&empty_ph, 0, monitor.tamanoBuffer);
  sem_init(&full_ph, 0, 0);
  sem_init(&mutex_ph, 0, 1);

  // Crear hilos
  pthread_t recolector_thread, ph_thread, temperature_thread;
  pthread_create(&recolector_thread, NULL, recolector, (void*)monitor.nombrePipe);
  pthread_create(&ph_thread, NULL, ph_handler, (void*)monitor.archivoPH);
  pthread_create(&temperature_thread, NULL, temperature_handler, (void*)monitor.archivoTemp);

  // Esperar a que los hilos terminen
  pthread_join(recolector_thread, NULL);
  pthread_join(ph_thread, NULL);
  pthread_join(temperature_thread, NULL);

  // Destruir semáforos
  sem_destroy(&empty_temperature);
  sem_destroy(&full_temperature);
  sem_destroy(&mutex_temperature);
  sem_destroy(&empty_ph);
  sem_destroy(&full_ph);
  sem_destroy(&mutex_ph);

  return 0;
}

void llenarDatos(struct Monitor *monitor, char instruccion[], char contenido[]){
  if(strcmp(instruccion, "-b") == 0){
    monitor->tamanoBuffer = atoi(contenido);
  }  
  if(strcmp(instruccion, "-t") == 0){
    strcpy(monitor->archivoTemp, contenido);
  }
  if(strcmp(instruccion, "-h") == 0){
    strcpy(monitor->archivoPH, contenido);
  }
  if(strcmp(instruccion, "-p") == 0){
    strcpy(monitor->nombrePipe, contenido);
  }
}

// Función para el hilo recolector
void *recolector(void *arg) {
  char *pipe_name = (char *)arg;
  // Abrir el pipe nominal para lectura
  int pipe_fd = open(pipe_name, O_RDONLY);
  if (pipe_fd == -1) {
    perror("Error al abrir el pipe nominal para lectura");
    exit(EXIT_FAILURE);
  }

    // Leer del pipe nominal y colocar en buffers
  while (1) {
    char receptor[100]; // Ajustar el tamaño según sea necesario
    if (read(pipe_fd, receptor, sizeof(buffer)) == -1) {
        perror("Error al leer del pipe nominal");
        exit(EXIT_FAILURE);
    }
    // Procesar la medición y colocar en los buffers
    char *dato = strtok(receptor, "_");
    char *tipo = strtok(NULL, "_");

    // Interpretar la medición y colocarla en el buffer correspondiente
    int sensor_type = atoi(tipo);
    float value = atof(dato); // Convertir la cadena a un valor flotante
    struct Buffer buffer;
    buffer.contTemp = 0;
    buffer.contPH = 0;
    
    if(value >= 0) {
      if (sensor_type == 1) {
          sem_wait(&empty_temperature);
          sem_wait(&mutex_temperature);

          buffer.temperatura[buffer.contTemp++] = value;

          sem_post(&mutex_temperature);
          sem_post(&full_temperature);
      } 
      else if (sensor_type == 2) {
            sem_wait(&empty_ph);
            sem_wait(&mutex_ph);

            buffer.ph[buffer.contPH++] = value;

            sem_post(&mutex_ph);
            sem_post(&full_ph);
      }      
    } 
    else {
        printf("Valor negativo descartado: %f\n", value);
    }
  }

    // Cerrar el pipe nominal
    close(pipe_fd);

    return NULL;
}

// Función para el hilo H-ph
void *ph_handler(void *arg) {
    char *ph_file_name = (char *)arg;

    // Abrir el archivo de salida para escribir mediciones de pH
    FILE *ph_file = fopen(ph_file_name, "w");
    if (ph_file == NULL) {
        perror("Error al abrir el archivo de salida para mediciones de pH");
        exit(EXIT_FAILURE);
    }

    // Leer del buffer y escribir en el archivo de salida
    while (1) {
        sem_wait(&full_ph);
        sem_wait(&mutex_ph);

        float ph_value = buffer.ph[--buffer.ph_count];

        sem_post(&mutex_ph);
        sem_post(&empty_ph);

        // Obtener la hora actual
        time_t raw_time;
        struct tm *time_info;
        char time_buffer[80];
        time(&raw_time);
        time_info = localtime(&raw_time);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

        // Escribir la medición de pH en el archivo de salida con la hora actual
        fprintf(ph_file, "[%s] pH: %.1f\n", time_buffer, ph_value);

        // Imprimir mensaje de alerta si el valor de pH está fuera de los rangos
        if (ph_value < 6.0 || ph_value > 8.0) {
            printf("Alerta: Valor de pH fuera de rango: %.1f\n", ph_value);
        }
    }

    // Cerrar el archivo de salida
    fclose(ph_file);

    return NULL;
}

// Función para el hilo H-temperatura
void *temperature_handler(void *arg) {
    char *temperature_file_name = (char *)arg;

    // Abrir el archivo de salida para escribir mediciones de temperatura
    FILE *temperature_file = fopen(temperature_file_name, "w");
    if (temperature_file == NULL) {
        perror("Error al abrir el archivo de salida para mediciones de temperatura");
        exit(EXIT_FAILURE);
    }

    // Leer del buffer y escribir en el archivo de salida
    while (1) {
        sem_wait(&full_temperature);
        sem_wait(&mutex_temperature);

        float temperature_value = buffer.temperature[--buffer.temperature_count];

        sem_post(&mutex_temperature);
        sem_post(&empty_temperature);

        // Obtener la hora actual
        time_t raw_time;
        struct tm *time_info;
        char time_buffer[80];
        time(&raw_time);
        time_info = localtime(&raw_time);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

        // Escribir la medición de temperatura en el archivo de salida con la hora actual
        fprintf(temperature_file, "[%s] Temperatura: %.1f\n", time_buffer, temperature_value);

        // Imprimir mensaje de alerta si el valor de temperatura está fuera de los rangos
        if (temperature_value < 20.0 || temperature_value > 31.6) {
            printf("Alerta: Valor de temperatura fuera de rango: %.1f\n", temperature_value);
        }
    }

    // Cerrar el archivo de salida
    fclose(temperature_file);

    return NULL;
}
