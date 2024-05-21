 /*********************************************************************
Proyecto: Monitoreo de Sensores
Autores: Juan Felipe Galvis, Andrés Pérez, Juan Diego Reyes
Fecha: 21 de Mayo de 2024
Materia: Sistemas Operativos
Pontificia Universidad Javeriana
Objetivo: Crear un sistema de monitoreo de datos de sensores de PH y
          temperatura que utiliza un buffer circular, semáforos e hilos
          para recolectar y registrar datos en archivos 
**************************************************************************/

// Proceso Monitor //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>

// Tamaño por defecto del buffer, se actualiza cuando se corre el monitor
#define DEFAULT_BUFFER_SIZE 10 

// Estructura que define el buffer circular
typedef struct {
    float *buffer; // Buffer circular para almacenar valores
    int size; // Tamaño del buffer
    int in; // Índice de entrada
    int out; // Índice de salida
    sem_t full; // Semáforo para contar elementos llenos
    sem_t empty; // Semáforo para contar espacios vacíos
    pthread_mutex_t mutex; // Mutex para sincronizar el acceso al buffer
} Buffer;

Buffer ph_buffer; // Buffer para valores de PH
Buffer temp_buffer; // Buffer para valores de temperatura
int pipe_fd; // Descriptor de archivo para el pipe
bool monitor_running = true; // Variable global para indicar si el monitor está en ejecución

// Función para inicializar un buffer
void init_buffer(Buffer *buffer, int size) {
    buffer->size = size; // Inicializa el tamaño del buffer
    buffer->buffer = (float *)malloc(sizeof(float) * size); // Reserva memoria para el buffer
    buffer->in = 0; // Inicializa el índice de entrada
    buffer->out = 0; // Inicializa el índice de salida
    sem_init(&buffer->full, 0, 0); // Inicializa el semáforo de elementos llenos a 0
    sem_init(&buffer->empty, 0, size); // Inicializa el semáforo de espacios vacíos al tamaño del buffer
    pthread_mutex_init(&buffer->mutex, NULL); // Inicializa el mutex
}

// Función para liberar la memoria del buffer y destruir los semáforos y el mutex
void free_buffer(Buffer *buffer) {
    free(buffer->buffer); // Libera la memoria del buffer
    sem_destroy(&buffer->full); // Destruye el semáforo de elementos llenos
    sem_destroy(&buffer->empty); // Destruye el semáforo de espacios vacíos
    pthread_mutex_destroy(&buffer->mutex); // Destruye el mutex
}

// Función para recolectar datos del pipe
void *recolector(void *arg) {
    while (monitor_running) {
        int tipo_sensor;
        float value;
        ssize_t num_bytes = read(pipe_fd, &tipo_sensor, sizeof(int)); // Lee el tipo de sensor del pipe
        if (num_bytes <= 0) {
            // No hay más datos o se cerró el pipe
            usleep(10000); // Dormir por un breve momento antes de intentar leer de nuevo
            continue;
        }
        read(pipe_fd, &value, sizeof(float)); // Lee el valor del sensor del pipe

        if (value < 0) {
            printf("Valor erróneo recibido: %.2f\n", value); // Imprime un mensaje si el valor es erróneo
            continue;
        }

        Buffer *buffer = (tipo_sensor == 1) ? &temp_buffer : &ph_buffer; // Selecciona el buffer correspondiente

        sem_wait(&buffer->empty); // Espera un espacio vacío en el buffer
        pthread_mutex_lock(&buffer->mutex); // Bloquea el mutex
        buffer->buffer[buffer->in] = value; // Escribe el valor en el buffer
        buffer->in = (buffer->in + 1) % buffer->size; // Actualiza el índice de entrada
        pthread_mutex_unlock(&buffer->mutex); // Desbloquea el mutex
        sem_post(&buffer->full); // Incrementa el semáforo de elementos llenos
    }

    printf("Recolector ha terminado de recibir datos.\n");
    return NULL;
}

// Función para escribir una medición en un archivo
void write_measurement(FILE *file, float value) {
    time_t now = time(NULL); // Obtiene el tiempo actual
    struct tm *gmt_time = gmtime(&now); // Convierte el tiempo a GMT
    gmt_time->tm_hour -= 5; // Ajusta la hora a GMT-5
    mktime(gmt_time); // Normaliza la estructura de tiempo

    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", gmt_time); // Formatea la hora
    fprintf(file, "%s: %.2f\n", time_str, value); // Escribe la medición en el archivo
    fflush(file); // Asegura de que los datos se escriban en el archivo inmediatamente
}

// Función para manejar los datos de PH
void *h_ph(void *arg) {
    bool print_title = false;
    char *file_name = (char *)arg;
    FILE *file = fopen(file_name, "a"); // Abrir archivo en modo append
    if (file == NULL) {
        perror("fopen");
        return NULL;
    }
    if(!print_title){
        fprintf(file, "\tResultados de sensores de Ph\n\n");
        print_title = true;
    }
    while (monitor_running || sem_trywait(&ph_buffer.full) == 0) {
        if (!monitor_running && sem_trywait(&ph_buffer.full) != 0) {
            break; // Salir del ciclo si el monitor ha terminado y el buffer está vacío
        }

        sem_wait(&ph_buffer.full); // Espera un elemento lleno en el buffer
        pthread_mutex_lock(&ph_buffer.mutex); // Bloquea el mutex
        float value = ph_buffer.buffer[ph_buffer.out]; // Lee el valor del buffer
        ph_buffer.out = (ph_buffer.out + 1) % ph_buffer.size; // Actualiza el índice de salida
        pthread_mutex_unlock(&ph_buffer.mutex); // Desbloquea el mutex
        sem_post(&ph_buffer.empty); // Incrementa el semáforo de espacios vacíos

        if (value < 6 || value > 8) {
            printf("Alerta: PH fuera de rango: %.2f\n", value); // Imprime un mensaje si el PH está fuera de rango
        }
        write_measurement(file, value); // Escribe la medición en el archivo
    }
    fclose(file); // Cierra el archivo
    return NULL;
}

// Función para manejar los datos de temperatura
void *h_temperatura(void *arg) {
    bool print_title = false;
    char *file_name = (char *)arg;
    FILE *file = fopen(file_name, "a"); // Abrir archivo en modo append
    if (file == NULL) {
        perror("fopen");
        return NULL;
    }
    if(!print_title){
        fprintf(file, "\tResultados de sensores de Temperatura\n\n");
        print_title = true;
    }
    while (monitor_running || sem_trywait(&temp_buffer.full) == 0) {
        if (!monitor_running && sem_trywait(&temp_buffer.full) != 0) {
            break; // Salir del ciclo si el monitor ha terminado y el buffer está vacío
        }

        sem_wait(&temp_buffer.full); // Espera un elemento lleno en el buffer
        pthread_mutex_lock(&temp_buffer.mutex); // Bloquea el mutex
        float value = temp_buffer.buffer[temp_buffer.out]; // Lee el valor del buffer
        temp_buffer.out = (temp_buffer.out + 1) % temp_buffer.size; // Actualiza el índice de salida
        pthread_mutex_unlock(&temp_buffer.mutex); // Desbloquea el mutex
        sem_post(&temp_buffer.empty); // Incrementa el semáforo de espacios vacíos

        if (value < 20 || value > 31.6) {
            printf("Alerta: Temperatura fuera de rango: %.2f\n", value); // Imprime un mensaje si la temperatura está fuera de rango
        }
        write_measurement(file, value); // Escribe la medición en el archivo
    }
    fclose(file); // Cierra el archivo
    return NULL;
}

// Función para imprimir el uso del programa
void print_usage() {
    printf("Usage: ./monitor -b tam_buffer -t file_temp -h file_ph -p pipe_nominal\n");
}

int main(int argc, char *argv[]) {
    int tam_buffer = 0; // Variable para el tamaño del buffer
    char *file_temp = NULL; // Archivo para los datos de temperatura
    char *file_ph = NULL; // Archivo para los datos de PH
    char *pipe_nominal = NULL; // Nombre del pipe

    int opt;
    // Procesar las opciones de línea de comandos
    while ((opt = getopt(argc, argv, "b:t:h:p:")) != -1) {
        switch (opt) {
            case 'b':
                tam_buffer = atoi(optarg); // Convierte el argumento de tamaño del buffer a entero
                break;
            case 't':
                file_temp = optarg; // Asigna el argumento de archivo de temperatura a la variable correspondiente
                break;
            case 'h':
                file_ph = optarg; // Asigna el argumento de archivo de PH a la variable correspondiente
                break;
            case 'p':
                pipe_nominal = optarg; // Asigna el argumento de pipe a la variable correspondiente
                break;
            default:
                print_usage(); // Imprime el uso correcto si hay una opción inválida
                exit(EXIT_FAILURE);
        }
    }

    // Verifica que todos los argumentos requeridos han sido proporcionados
    if (tam_buffer == 0 || file_temp == NULL || file_ph == NULL || pipe_nominal == NULL) {
        print_usage(); // Imprime el uso correcto si falta algún argumento
        exit(EXIT_FAILURE);
    }

    // Establecer la zona horaria a la de Colombia
    setenv("TZ", "America/Bogota", 1);
    tzset();

    init_buffer(&ph_buffer, tam_buffer); // Inicializa el buffer de PH con el tamaño especificado
    init_buffer(&temp_buffer, tam_buffer); // Inicializa el buffer de temperatura con el tamaño especificado

    // Crea el pipe nombrado
    if (mkfifo(pipe_nominal, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Abre el pipe en modo lectura y no bloqueante
    pipe_fd = open(pipe_nominal, O_RDONLY | O_NONBLOCK);
    if (pipe_fd == -1) {
        perror("open pipe");
        unlink(pipe_nominal);
        exit(EXIT_FAILURE);
    }

    pthread_t recolector_thread, ph_thread, temp_thread; // Variables para los threads
    pthread_create(&recolector_thread, NULL, recolector, pipe_nominal); // Crea el thread para el recolector
    pthread_create(&ph_thread, NULL, h_ph, file_ph); // Crea el thread para manejar los datos de PH
    pthread_create(&temp_thread, NULL, h_temperatura, file_temp); // Crea el thread para manejar los datos de temperatura

    pthread_join(recolector_thread, NULL); // Espera a que termine el thread del recolector
    monitor_running = false; // Indica que el monitor ha terminado
    pthread_join(ph_thread, NULL); // Espera a que termine el thread de PH
    pthread_join(temp_thread, NULL); // Espera a que termine el thread de temperatura

    close(pipe_fd); // Cierra el pipe
    unlink(pipe_nominal); // Elimina el pipe

    free_buffer(&ph_buffer); // Libera la memoria del buffer de PH
    free_buffer(&temp_buffer); // Libera la memoria del buffer de temperatura

    return 0;
}
