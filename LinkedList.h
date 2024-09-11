#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template <typename T>
class LinkedList {
private:
    struct Node {
        T* address;   // Dirección de memoria
        int id;       // ID único para cada nodo
        int refCount; // Contador de referencias
        Node* next;   // Siguiente nodo en la lista

        // Constructor del nodo
        Node(T* addr, int idVal) : address(addr), id(idVal), refCount(1), next(nullptr) {}
    };

    Node* head;        // Puntero al inicio de la lista
    int currentId;     // Contador para generar IDs únicos

public:
    LinkedList() : head(nullptr), currentId(0) {}

    // Encontrar un nodo por dirección
    Node* find(T* address);

    // Encontrar un nodo por ID
    Node* findById(int id);

    // Insertar una nueva dirección en la lista
    void insert(T* address, int& newId);

    // Eliminar un nodo por ID (sin liberar memoria)
    void remove(int id);

    // Obtener la dirección asociada a un ID
    T* getAddressById(int id);

    // Obtener el refCount de un nodo por ID
    int getRefCountById(int id);

    // Actualizar el refCount de un nodo por ID
    void setRefCountById(int id, int refCount);

    int getCurrentId() const {
        return currentId;
    }

    // Destructor (no elimina la memoria apuntada)
    ~LinkedList();
};

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::find(T* address) {
    Node* current = head;
    while (current != nullptr) {
        if (current->address == address) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::findById(int id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

template <typename T>
void LinkedList<T>::insert(T* address, int& newId) {
    Node* newNode = new Node(address, ++currentId);
    newId = newNode->id;
    newNode->next = head;
    head = newNode;
}

template <typename T>
void LinkedList<T>::remove(int id) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            if (previous) {
                previous->next = current->next;
            } else {
                head = current->next;
            }
            delete current;  // Solo elimina el nodo
            return;
        }
        previous = current;
        current = current->next;
    }
}

template <typename T>
T* LinkedList<T>::getAddressById(int id) {
    Node* node = findById(id);
    return (node != nullptr) ? node->address : nullptr;
}

template <typename T>
int LinkedList<T>::getRefCountById(int id) {
    Node* node = findById(id);
    return (node != nullptr) ? node->refCount : 0;
}

template <typename T>
void LinkedList<T>::setRefCountById(int id, int refCount) {
    Node* node = findById(id);
    if (node) {
        node->refCount = refCount;
    }
}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;  // Solo elimina los nodos
    }
}

#endif // LINKEDLIST_H
