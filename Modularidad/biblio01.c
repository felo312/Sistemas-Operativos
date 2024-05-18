/*fecha:30/01/2024 
  Autor: Juan Felipe Galvis Vargas
  Tema: Introudcción C
  Tópico: Compilación por separado
  Materia: Sistemas Operativos
  Fichero: Biblioteca de funciones
*/
#include "biblio01.h"
#include <stdio.h>

#define PI 3.1416

int sumar(int a, int b) {
    int resultado = a + b;
    printf("Suma de %d y %d: %d\n", a, b, resultado);
    return resultado;
}

int calcularExponencial(int base, int exponente) {
    int resultado = 1;
    for (int i = 0; i < exponente; i++) {
        resultado *= base;
    }
    printf("%d elevado a la %d: %d\n", base, exponente, resultado);
    return resultado;
}

int calcularAreaRectangulo(int longitud, int ancho) {
    int resultado = longitud * ancho;
    printf("Área de un rectángulo con longitud %d y ancho %d: %d\n", longitud, ancho, resultado);
    return resultado;
}

float calcularAreaCirculo(int radio) {
    float resultado = PI * radio * radio;
    printf("Área de un círculo con radio %d: %.4f\n", radio, resultado);
    return resultado;
}

