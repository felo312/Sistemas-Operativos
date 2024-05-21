 /*********************************************************************
Proyecto: Monitoreo de Sensores
Autores: , Juan Felipe Galvis, Andrés Pérez, Juan Diego Reyes
Fecha: 21 de Mayo de 2024
Materia: Sistemas Operativos
Pontificia Universidad Javeriana
Objetivo: Crear un archivo makefile para automatizar la compilación.
**************************************************************************/

CC=gcc
CFLAGS=-Wall -pthread
TARGETS=sensor monitor

all: $(TARGETS)

sensor: sensor.c
	$(CC) $(CFLAGS) -o sensor sensor.c

monitor: monitor.c
	$(CC) $(CFLAGS) -o monitor monitor.c

clean:
	rm -f $(TARGETS)
	rm -f *.o
