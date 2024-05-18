/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Fecha: 13 febrero 2024
Materia: Sistemas Operativos
Tema: Uso de puntetos en C
*************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int arreglo[3];
  int *p = arreglo;
  int *ref= &arreglo[2];
  
  if(argc <= 3){ 
    printf("Entrada de argumentos inválidos\n");
    printf("$./ejecutable arg1 arg2 arg3\n"); 
  } else{ 
        arreglo[0]= atoi(argv[1]);
        arreglo[1]= atoi(argv[2]);
        arreglo[2]= atoi(argv[3]);
        }  
  
  for(int i = 0; i < 3; i++, *p++){ 
    printf("%d\n", *p);
    printf("%d\n", *ref);
  } 
  return 0;
}


/*    printf("Valor 1 ingresado es: %d\n", valor);
printf("Valor 2 ingresado es: %d\n", valor2);
printf("Valor 3 ingresado es: %d\n\n", valor3);

} else{ 
    printf("Entrada de argumentos inválidos\n");
    printf("$./ejecutable arg1 arg2 arg3\n");
    }  */