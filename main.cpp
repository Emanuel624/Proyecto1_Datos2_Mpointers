#include <iostream>
#include "MPointer.h"

int main() {
    MPointer<int> myPtr = MPointer<int>::New();
    *myPtr = 5;
    int valor1 = &myPtr;
    std::cout << "Valor en myPtr: " << valor1 << std::endl;

    MPointer<int> myPtr2 = MPointer<int>::New();
    myPtr2 = myPtr; // Ahora myPtr2 debe apuntar a la misma dirección que myPtr
    int valor2 = &myPtr2;
    std::cout << "Valor en myPtr2 (después de asignar myPtr): " << valor2 << std::endl;

    myPtr2 = 10; // Esto debe asignar 10 al puntero interno de myPtr2
    std::cout << "Nuevo valor en myPtr2: " << valor2 << std::endl;
    std::cout << "Valor en myPtr (después de asignar 10 a myPtr2): " << *myPtr << std::endl;

    return 0;
}
