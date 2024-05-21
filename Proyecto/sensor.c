 /*********************************************************************
Proyecto: Monitoreo de Sensores
Autores: , Juan Felipe Galvis, Andrés Pérez, Juan Diego Reyes
Fecha: 21 de Mayo de 2024
Materia: Sistemas Operativos
Pontificia Universidad Javeriana
Objetivo: Crear un sistema de monitoreo de datos de sensores de PH y
          temperatura que utiliza un buffer circular, semáforos e hilos
          para recolectar y registrar datos en archivos 
**************************************************************************/

// Proceso sensor //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Función que imprime el uso correcto del programa
void print_usage() {
    printf("Uso del programa: ./sensor -s tipo_sensor -t tiempo -f archivo -p pipe_nominal\n\nSe pueden poner en cualquier orden");
}

int main(int argc, char *argv[]) {
    int tipo_sensor = 0;       // Variable para almacenar el tipo de sensor
    int tiempo = 0;            // Variable para almacenar el tiempo de espera entre lecturas
    char *archivo = NULL;      // Puntero para el nombre del archivo de datos del sensor
    char *pipe_nominal = NULL; // Puntero para el nombre del pipe

    int opt;                   // Variable para el retorno de getopt
    // Bucle para procesar las opciones de línea de comandos
    while ((opt = getopt(argc, argv, "s:t:f:p:")) != -1) {
        switch (opt) {
            case 's':
                tipo_sensor = atoi(optarg); // Convierte el argumento de tipo de sensor a entero
                break;
            case 't':
                tiempo = atoi(optarg);      // Convierte el argumento de tiempo a entero
                break;
            case 'f':
                archivo = optarg;           // Asigna el argumento de archivo a la variable correspondiente
                break;
            case 'p':
                pipe_nominal = optarg;      // Asigna el argumento de pipe a la variable correspondiente
                break;
            default:
                print_usage();              // Imprime el uso correcto si hay una opción inválida
                exit(EXIT_FAILURE);
        }
    }

    // Verifica que todos los argumentos requeridos han sido proporcionados
    if (tipo_sensor == 0 || tiempo == 0 || archivo == NULL || pipe_nominal == NULL) {
        print_usage(); // Imprime el uso correcto si falta algún argumento
        exit(EXIT_FAILURE);
    }

    // Abre el pipe en modo escritura
    int pipe_fd = open(pipe_nominal, O_WRONLY);
    if (pipe_fd == -1) {
        perror("open pipe"); // Imprime un mensaje de error si falla la apertura del pipe
        exit(EXIT_FAILURE);
    }

    // Abre el archivo de datos del sensor en modo lectura
    FILE *file = fopen(archivo, "r");
    if (file == NULL) {
        perror("fopen"); // Imprime un mensaje de error si falla la apertura del archivo
        close(pipe_fd);
        exit(EXIT_FAILURE);
    }

    char line[256]; // Buffer para almacenar las líneas leídas del archivo
    // Bucle para leer las líneas del archivo
    while (fgets(line, sizeof(line), file)) {
        float value = atof(line); // Convierte la línea leída a un valor flotante
        // Escribe el tipo de sensor y el valor en el pipe
        write(pipe_fd, &tipo_sensor, sizeof(int));
        write(pipe_fd, &value, sizeof(float));
        sleep(tiempo); // Espera el tiempo especificado antes de la próxima lectura
    }

    fclose(file); // Cierra el archivo
    close(pipe_fd); // Cierra el pipe

    return 0; // Termina el programa con éxito
}