/*************************************
Pontificia Universidad Javeriana
Autor: Juan Felipe Galvis Vargas
Co-autor: Andrés David Pérez
Fecha: 30 abril 2024
Materia: Sistemas Operativos
Tema: Interfaz de las funciones
*************************************/

//definición nombre biblioteca
#ifndef __BIBLIO_H__
#define __BIBLIO_H__

// Se requiere una estructura para almacenar los datos de entrada
struct datosEntrada {
  int edad, genero;
  float peso, estatura;
  char *fichero; //nombre del fichero a buscar
};

//función para identificar banderas con argumentos
void obtFlag(int argc, char *argv[], struct datosEntrada *datos);

#endif
