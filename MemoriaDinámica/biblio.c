/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Fecha: 05 marzo 2024
Materia: Sistemas Operativos
Tema: Bonus memoria dinámica y punteros
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "biblio.h"

void ej1() {
    char *p = NULL;
    printf("Direccion de p = %p\n", (void *)p);
    p = (char *)malloc(strlen("Hello, Im the best in operating system!") + 1);
    strcpy(p, "Hello, Im the best in operating system!");
    printf("%s\n", p);
    printf("Copiando \"Goodbye\" to p\n");
    strcpy(p, "Goodbye");
    printf("String copiado\n");
    printf("%s\n", p);
    free(p);
}

void ej2() {
    char *q = NULL;
    printf("Pidiendo espacio para \"Goodbye\"\n");
    q = (char *)malloc(strlen("Goodbye") + 1);
    if (!q) {
        perror("Failed to allocate space because");
        exit(1);
    }
    //perror("Failed to allocate space because");
    printf("About to copy \"Goodbye\" to q at address %p\n", (void *)q);
    strcpy(q, "Goodbye");
    printf("String copied\n");
    printf("%s\n", q);
    free(q);
}

void ej3() {
    int *ptr;
    ptr = (int *)malloc(15 *sizeof(*ptr));
    if (ptr != NULL) {
        *(ptr+5)=480;
    }
    free(ptr);
}

void ej4() {
    int n, i, *ptr, sum = 0;
    printf("Ingrese numero de elementos:");
    if (scanf("%d", &n) != 1) {
        printf("No es valido\n");
        exit(1);
    }
    ptr = (int *)calloc(n, sizeof(int));
    if (ptr == NULL) {
        printf("Error!");
        exit(1);
    }
    printf("Ingrese los elementos: ");
    for (i = 0; i < n; ++i) {
        if (scanf("%d", ptr + i) != 1) {
            printf("Invalido\n");
            exit(1);
        }
        sum += *(ptr + i);
    }
    printf("Sum = %d\n", sum);
    free(ptr);
}

void ej5() {
    int *ptr, i, n1, n2;
    printf("Ingrese tamaño: ");
    if (scanf("%d", &n1) != 1) {
        printf("Invalido\n");
        exit(1);
    }
    ptr = (int *)malloc(n1 * sizeof(int));
    printf("Dirección de la memoria previa: ");
    for (i = 0; i < n1; ++i)
        printf("%p\n", (void *)(ptr + i));

    printf("Ingrese tamaño: ");
    if (scanf("%d", &n2) != 1) {
        printf("Invalido\n");
        exit(1);
    }
    ptr = realloc(ptr, n2 * sizeof(int));
    printf("Direccion nueva memoria: ");
    for (i = 0; i < n2; ++i)
        printf("%p\n", (void *)(ptr + i));
    free(ptr);
}

void ej6() {
    char *a = NULL;
    char *b = NULL;
    a = function("Divertido aprender");
    b = function("systems engineer");
    printf("Del main: %s %s\n", a, b);
    free(a);
    free(b);
}

char *function(char *p) {
    size_t len = strlen(p);
    char *q = (char *)malloc(len + 1);
    strcpy(q, p);
    printf("From function: the string is %s\n", q);
    return q;
}

void ej7() {
    int variable = 123;
    int *ptr = &variable;
    printf("Valor variable = %d \n", variable);
    printf("Direccion memoria variable = %p\n", (void *)&variable);
    printf("Direccion a la que apunta el puntero = %p \n", (void *)ptr);
    printf("Valor variable = %d \n", *ptr);
}

void ej8() {
    #define size 10
    int *vectorPunteros[3];
    int p = 40, q = 80, r = 120;
    vectorPunteros[0] = &p;
    vectorPunteros[1] = &q;
    vectorPunteros[2] = &r;
    printf("\nForma de acceso al vector de punteros:\n");
    for (int i = 0; i < 3; i++)
        printf("Direccion: %p \t el valor = %d\n", (void *)vectorPunteros[i], *vectorPunteros[i]);
}

void ej9() {
    #define size 10
    int value1, value2;
    int *ptrV1, *ptrV2;
    ptrV1 = &value1;
    ptrV2 = &value2;
    printf("\nIngrese 2 valores a sumar: ");
    if (scanf("%d %d", &value1, &value2) != 2) {
        printf("Invalido\n");
        exit(1);
    }
    printf("\nLa suma es = %d\n", *ptrV1 + *ptrV2);
}

void ej10() {
    int vector[] = {40, 80, 120, 230};
    int *ptrV1 = vector;
    printf("\nImpresion por indices:\n");
    printf("---------------------\n");
    for (int i = 0; i < 4; i++)
        printf("Por indices: el valor del vector[%d] = %d\n", i, ptrV1[i]);
    printf("\nImpresion por sucesor\n");
    printf("---------------------\n");
    for (int i = 0; i < 4; i++) {
        printf("Por sucesor: el valor del vector[%d] = %d\n", i, *ptrV1);
        ptrV1++;
    }
}

void ej11() {
    int var = 480;
    int *varptr = &var;
    int **doubleptr = &varptr;
    printf("Valor de la variable \t\t= %d\n", var);
    printf("Valor del puntero \t\t= %d\n", *varptr);
    printf("Valor del puntero doble \t= %d \n", **doubleptr);
    
    printf("Direccion de la variable \t= %p\n", (void *)&var);
    
    printf("Direccion del punteroo \t\t= %p\n", (void *)&varptr);
    printf("Valor del puntero  \t\t= %p\n", (void *)varptr);
    
    printf("Direccion del puntero doble \t= %p\n", (void *)doubleptr);
    printf("Valor del puntero doble \t= %p\n", (void *)*doubleptr);
}

void ej12(int argc, char **argv) {
    int rows, cols, i, j;
    int **matrix;

    rows = (int)atof(argv[1]);
    cols = (int)atof(argv[2]);
    
    matrix = (int **)malloc(rows * sizeof(int *));
    for (i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            matrix[i][j] = i * j;
        }
    }

    for (i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
