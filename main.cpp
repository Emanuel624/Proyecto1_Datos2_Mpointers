#include <iostream>
#include "MPointer.h"
int main() {
    {
        MPointer<int> ptr1 = MPointer<int>::New();
        MPointer<int> ptr2 = MPointer<int>::New();

        *ptr1 = 5;
        //Llama a debug en MPointerGC para ber el estado
        MPointerGC<int>::getInstance()->debug();

        ptr2 = 666;

        MPointer<int>mp3 = ptr2; //Shallow copy (van a compartir la misma direccion de memoria) aumenta el refcount
        MPointerGC<int>::getInstance()->debug();

        int valor = &mp3;
        std::cout << valor <<std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));  // Pausa para lograr que el thread de GC se ejecute

    return 0;
}

