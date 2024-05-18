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

int main(int argc, char **argv) {
    
    ej1();
    ej2();
    ej3();
    ej4();
    ej5();
    ej6();
    ej7();
    ej8();
    ej9();
    ej10();
    ej11();
    if(argc>=3){
        ej12(argc, argv); //para las filas y colum
    }
    return 0;
}

//gcc mm_bonos.c biblio.c -o ex1