#include <iostream>
#include "MPointer.h"

int main() {
    // Ejemplo 1: Creación de un MPointer y asignación de valor
    MPointer<int> myPtr = MPointer<int>::New();
    *myPtr = 5;
    std::cout << "Valor en myPtr: " << *myPtr << std::endl;

    // Ejemplo 2: Asignación de un MPointer a otro
    MPointer<int> myPtr2 = MPointer<int>::New();
    myPtr2 = myPtr; // Ahora myPtr2 debe apuntar a la misma dirección que myPtr
    std::cout << "Valor en myPtr2 (después de asignar myPtr): " << *myPtr2 << std::endl;

    // Ejemplo 3: Asignación de un valor directo
    myPtr2 = 10; // Esto debe asignar 10 al puntero interno de myPtr2
    std::cout << "Nuevo valor en myPtr2: " << *myPtr2 << std::endl;
    std::cout << "Valor en myPtr (después de asignar 10 a myPtr2): " << *myPtr << std::endl;

    return 0;
}
