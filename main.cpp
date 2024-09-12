#include <iostream>
#include <stdexcept> //Para el Throw
#include "MPointer.h"

// Nodo de la lista enlazada simple
template <typename T>
class Node {
public:
    T data;
    MPointer<Node> next = nullptr;  // MPointer para gestionar el siguiente nodo

    Node() = default;  // Constructor por defecto
    Node(T value) : data(value), next(nullptr) {}  // Constructor con valor
};

// Lista enlazada simple
template <typename T>
class SinglyLinkedList {
private:
    MPointer<Node<T>> head = nullptr;  // Primer nodo de la lista

    // Metodo para obtener un nodo en una posición específica
    Node<T>* getNodeAt(int index) {
        MPointer<Node<T>> current = head;
        int count = 0;
        while (current != nullptr && count < index) {
            current = current->next;
            count++;
        }
        return current.get();
    }

public:
    SinglyLinkedList() : head(nullptr) {}

    // Insertar un nuevo elemento al final de la lista
    void append(T value) {
        MPointer<Node<T>> newNode = MPointer<Node<T>>::New();  // Crea un nuevo MPointer para Node
        newNode->data = value;
        newNode->next = nullptr;

        if (head == nullptr) {  // Si la lista está vacía
            head = newNode;
        } else {  // Si la lista no está vacía
            MPointer<Node<T>> current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;  // Añadir el nuevo nodo al final
        }
    }

    // Obtener el tamaño de la lista
    int size() const {
        int count = 0;
        MPointer<Node<T>> current = head;
        while (current != nullptr) {
            current = current->next;
            count++;
        }
        return count;
    }

    // Acceder al valor en una posición específica
    T get(int index) {
        Node<T>* node = getNodeAt(index);
        if (node) {
            return node->data;
        }
        throw std::out_of_range("Índice fuera de rango");
    }

    // Establecer el valor en una posición específica
    void set(int index, T value) {
        Node<T>* node = getNodeAt(index);
        if (node) {
            node->data = value;
        } else {
            throw std::out_of_range("Índice fuera de rango");
        }
    }

    // Intercambiar dos nodos
    void swap(int i, int j) {
        T temp = get(i);
        set(i, get(j));
        set(j, temp);
    }
};

// Función auxiliar para particionar la lista
template <typename T>
int partition(SinglyLinkedList<T>& list, int low, int high) {
    // Escoge el último elemento como pivote
    T pivot = list.get(high);

    int i = low - 1;  // Índice del elemento más pequeño

    for (int j = low; j <= high - 1; j++) {
        if (list.get(j) < pivot) {
            i++;
            list.swap(i, j);
        }
    }
    list.swap(i + 1, high);
    return i + 1;
}

// Función recursiva de QuickSort
template <typename T>
void quickSort(SinglyLinkedList<T>& list, int low, int high) {
    if (low < high) {
        int pi = partition(list, low, high);  // Índice de partición
        quickSort(list, low, pi - 1);  // Ordena los elementos antes de la partición
        quickSort(list, pi + 1, high);  // Ordena los elementos después de la partición
    }
}

// Función para iniciar el QuickSort en la lista
template <typename T>
void quickSort(SinglyLinkedList<T>& list) {
    quickSort(list, 0, list.size() - 1);
}

// Implementación de Bubble Sort
template <typename T>
void bubbleSort(SinglyLinkedList<T>& list) {
    int n = list.size();
    bool swapped;

    do {
        swapped = false;
        for (int i = 0; i < n - 1; i++) {
            if (list.get(i) > list.get(i + 1)) {
                list.swap(i, i + 1);
                swapped = true;
            }
        }
        n--;  // Reduce el rango después de cada iteración
    } while (swapped);
}

// Implementación de Insertion Sort
template <typename T>
void insertionSort(SinglyLinkedList<T>& list) {
    int n = list.size();

    for (int i = 1; i < n; i++) {
        T key = list.get(i);
        int j = i - 1;

        // Desplaza los elementos de la lista que son mayores que el valor clave (key) hacia la derecha
        while (j >= 0 && list.get(j) > key) {
            list.set(j + 1, list.get(j));
            j--;
        }
        list.set(j + 1, key);
    }
}



// Programa principal
int main() {
    {
        SinglyLinkedList<int> list;

        list.append(10);
        list.append(7);
        list.append(8);
        list.append(9);
        list.append(178);
        list.append(5);

        int n = list.size();
        insertionSort(list); //Cambiar por el algortimo por probar

        // Mostrar la lista ordenada
        for (int i = 0; i < n; i++) {
            std::cout << list.get(i) << " ";
        }

        std::cout << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));  // Pausa para lograr que el thread de GC se ejecute

    return 0;
}
