/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Fecha: 16 abril 2024
Materia: Sistemas Operativos
Tema: Pipes (tuberías para comunicar o enviar información entre procesos)
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
  pid_t pID;

//tamaño 2 por entrada y salida de la tubería 
  int tuberia[2];

//crear tubería
  if(pipe(tuberia)){
    printf("Error creando la tubería\n");
    return -1;
  }

//crear el fork para hacer bifurcación de procesos
  pID = fork ();
  if(pID == 0){ //Proceso hijo 
  //se cierra primero el otro extremo de la tubería
    close(tuberia[0]); //0 este extremo de tubería
    char *mensaje = "\n:) Mensaje enviado desde el proceso hijo :) \n";
    write(tuberia[1], mensaje, strlen(mensaje)+1); //se escribe al otro extremo [1]
    close(tuberia[1]);
    return EXIT_SUCCESS;
  }  
  else if (pID > 0){ //proceso padre
  //se cierra primero el otro extremo de la tubería
    close(tuberia[1]);
    char busMensaje[54]; //Creae bus de tamaño 14
    read(tuberia[0], busMensaje, sizeof(busMensaje));
    printf("Mensaje desde el hijo: \n\t %s \n", busMensaje);
  }
  else {
    printf("Error en la creación del PIPE");
    return EXIT_FAILURE;
  }

  return 0;  
}
// gcc pipe01.c -o exepipe