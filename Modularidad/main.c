#include <stdio.h>
#include "biblio01.h"

int main() {
    // Datos para las operaciones
    int numero1 = 5, numero2 = 3;
    int baseExponencial = 2;
    int exponenteExponencial = 3;
    int longitudRectangulo = 4, anchoRectangulo = 3;
    int radioCirculo = 2;

    // Realizar las operaciones
    int resultadoSuma = sumar(numero1, numero2);
    int resultadoExponencial = calcularExponencial(baseExponencial, exponenteExponencial);
    int resultadoAreaRectangulo = calcularAreaRectangulo(longitudRectangulo, anchoRectangulo);
    float resultadoAreaCirculo = calcularAreaCirculo(radioCirculo);

    return 0;
}
