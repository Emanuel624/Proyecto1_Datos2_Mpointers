#include <iostream>
#include <stdexcept> // Para manejar excepciones
#include "MPointer.h"
#include "DoubleLinkedLIst.h"


// Programa principal para probar la lista doblemente enlazada
int main() {
    {
        DoublyLinkedList<int> list;

        list.append(10);
        list.append(7);
        list.append(8);
        list.append(9);
        list.append(178);
        list.append(5);

        int n = list.size();
        insertionSort(list);  // Cambiar por el algoritmo a probar (bubbleSort, quickSort, insertionsort)

        // Mostrar la lista ordenada
        for (int i = 0; i < n; i++) {
            std::cout << list.get(i) << " ";
        }
        std::cout << std::endl;


        /*
        MPointer<int> mp1 = MPointer<int>::New();
        MPointer<int> mp2 = MPointer<int>::New();


        *mp1 = 100;
        mp2 = 55;

        MPointer<int> mp3 = mp2; //(shallow copy)

        MPointerGC<int>::getInstance()->debug();

        */
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));  // Pausa para que el GC se ejecute

    return 0;
}
