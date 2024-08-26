#include <iostream>
#include "MPointer.h"

int main() {
    // Inicializar explícitamente el MPointerGC antes de usar MPointer
    MPointerGC<int>::getInstance();  // Esto inicia el hilo del GC

    MPointer<int> myPtr = MPointer<int>::New();
    myPtr = 5;
    int valor1 = &myPtr;
    std::cout << "Valor en myPtr: " << valor1 << std::endl;

    return 0;
}

