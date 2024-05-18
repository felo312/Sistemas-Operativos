#!/usr/bin/perl
#**************************************************************
#     Pontificia Universidad Javeriana
#     Autor: Juan Felipe Galvis, Andrés David Pérez Cely, Juan Diego Reyes
#     Fecha: 07 mayo 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

# Configuración inicial del script
$Path = `pwd`; # Obtiene el directorio actual
chomp($Path); # Elimina el salto de línea del directorio

# Definición de variables
$Nombre_Ejecutable = "clasico"; # Nombre del ejecutable a utilizar
@Size_Matriz = ("4000"); # Tamaños de las matrices a procesar "200","300","500","750","1000","1500","2000","3000",
@Num_Hilos = (1,2,4,8,16); # Números de hilos a utilizar
$Repeticiones = 30; # Número de repeticiones por configuración

# Bucle para recorrer los tamaños de las matrices
foreach $size (@Size_Matriz){
    # Bucle para recorrer los números de hilos
  foreach $hilo (@Num_Hilos) {
        # Creación del nombre del archivo de salida
    $file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";
        # Bucle para realizar las repeticiones
    for ($i=0; $i<$Repeticiones; $i++) {
            # Ejecución del comando para correr el ejecutable y redirigir la salida al archivo
      system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");
            # Impresión del comando que se ejecutaría (comentado)
       # printf("$Path/$Nombre_Ejecutable $size $hilo \n");
    }
    close($file); # Cierra el archivo (aunque no es necesario porque no se abrió explícitamente)
    $p=$p+1; # Incrementa un contador (aunque no se utiliza posteriormente)
  }
}
