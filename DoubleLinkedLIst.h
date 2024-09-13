#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H
#include <stdexcept> // Para manejar excepciones
#include "MPointer.h"

// Definicion del nodo de la lista doblemente enlazada utilizando MPointers
template <typename T>
class Node {
public:
    T data;
    MPointer<Node<T>> next = nullptr;  // MPointer para el siguiente nodo
    MPointer<Node<T>> prev = nullptr;  // MPointer para el nodo anterior

    Node() = default;                   // Constructor por defecto
    Node(T value) : data(value), next(nullptr), prev(nullptr) {}  // Constructor con valor
};


// Lista doblemente enlazada
template <typename T>
class DoublyLinkedList {
private:
    MPointer<Node<T>> head = nullptr;  // Puntero al primer nodo de la lista
    MPointer<Node<T>> tail = nullptr;  // Puntero al último nodo de la lista

    // Metodo para obtener un nodo en una posición específica utilizando MPointer
    MPointer<Node<T>> getNodeAt(int index) {
        MPointer<Node<T>> current = head;
        int count = 0;
        while (current != nullptr && count < index) {
            current = current->next;
            count++;
        }
        return current;
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    // Insertar un nuevo elemento al final de la lista
    void append(T value) {
        MPointer<Node<T>> newNode = MPointer<Node<T>>::New();  // Crear nuevo nodo usando MPointer
        newNode->data = value;
        newNode->next = nullptr;

        if (head == nullptr) {  // Si la lista está vacía
            head = newNode;
            tail = newNode;
        } else {  // Si la lista no está vacía
            tail->next = newNode;   // El último nodo apunta al nuevo nodo
            newNode->prev = tail;   // El nuevo nodo apunta al anterior
            tail = newNode;         // El nuevo nodo se convierte en el último nodo
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

    // Acceder al valor en una posición específica utilizando MPointer
    T get(int index) {
        MPointer<Node<T>> node = getNodeAt(index);
        if (node != nullptr) {  // Cambiado a `!= nullptr`
            return node->data;
        }
        throw std::out_of_range("Índice fuera de rango");
    }

    // Establecer el valor en una posición específica utilizando MPointer
    void set(int index, T value) {
        MPointer<Node<T>> node = getNodeAt(index);
        if (node != nullptr) {  // Cambiado a `!= nullptr`
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

    // Destructor para liberar la memoria de los nodos
    ~DoublyLinkedList() {
        MPointer<Node<T>> current = head;
        while (current != nullptr) {
            MPointer<Node<T>> next = current->next;
            current->prev = nullptr;  // Romper la referencia al nodo anterior
            current = nullptr;  // El garbage collector de MPointer se encargará de liberar la memoria
            current = next;
        }
    }
};

// Particionar la lista
template <typename T>
int partition(DoublyLinkedList<T>& list, int low, int high) {
    T pivot = list.get(high);

    int i = low - 1;

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
void quickSort(DoublyLinkedList<T>& list, int low, int high) {
    if (low < high) {
        int pi = partition(list, low, high);
        quickSort(list, low, pi - 1);
        quickSort(list, pi + 1, high);
    }
}

// Función para iniciar el QuickSort en la lista
template <typename T>
void quickSort(DoublyLinkedList<T>& list) {
    quickSort(list, 0, list.size() - 1);
}

// Implementación de Bubble Sort
template <typename T>
void bubbleSort(DoublyLinkedList<T>& list) {
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
        n--;
    } while (swapped);
}

// Implementación de Insertion Sort
template <typename T>
void insertionSort(DoublyLinkedList<T>& list) {
    int n = list.size();

    for (int i = 1; i < n; i++) {
        T key = list.get(i);
        int j = i - 1;

        while (j >= 0 && list.get(j) > key) {
            list.set(j + 1, list.get(j));
            j--;
        }
        list.set(j + 1, key);
    }
}
#endif //DOUBLELINKEDLIST_H