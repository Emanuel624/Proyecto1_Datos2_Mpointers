#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template <typename T>
class LinkedList {
private:
    struct Node {
        T* address;
        int refCount;
        int id;
        Node* next;

        Node(T* addr, int idVal) : address(addr), refCount(1), id(idVal), next(nullptr) {}
    };

    Node* head;
    int currentId;

public:
    LinkedList() : head(nullptr), currentId(0) {}
    Node* find(T* address);
    Node* findById(int id);
    void insert(T* address, int& newId);
    void remove(int id);
    void decreaseRefCount(int id);
    void clean();
    T* getAddressById(int id);
    int getRefCountById(int id);
    void debug() const;
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
    return nullptr;  // Devuelve nullptr si el nodo no se encuentra
}

template <typename T>
void LinkedList<T>::insert(T* address, int& newId) {
    Node* newNode = new Node(address, ++currentId);
    newNode->refCount = 1;  // Initialize refCount to 1
    newId = newNode->id;
    newNode->next = head;
    head = newNode;
    std::cout << "Inserting node with ID: " << newId << " and refCount: " << newNode->refCount << std::endl;
}


template <typename T>
void LinkedList<T>::remove(int id) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            if (current->refCount == 0) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
                }
                std::cout << "Deleting node with id: " << id << std::endl;
                delete current->address;  // Libera la memoria apuntada
                delete current;  // Elimina el nodo de la lista
                return;  // Sale del bucle una vez que el nodo es eliminado
            }
        }
        previous = current;
        current = current->next;
    }
}


template <typename T>
void LinkedList<T>::decreaseRefCount(int id) {
    Node* node = findById(id);
    if (node) {
        if (node->refCount > 0) {
            node->refCount--;
        }
        if (node->refCount == 0) {
            remove(id);  // Si el refCount llega a 0, eliminamos el nodo
        }
    }
}

template <typename T>
void LinkedList<T>::clean() {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->refCount == 0) {
            if (previous) {
                previous->next = current->next;
            } else {
                head = current->next;
            }
            delete current->address;  // Libera la memoria asociada
            Node* temp = current;
            current = current->next;
            delete temp;  // Elimina el nodo de la lista
        } else {
            previous = current;
            current = current->next;
        }
    }
}

template <typename T>
T* LinkedList<T>::getAddressById(int id) {
    Node* node = findById(id);
    return (node != nullptr && node->address != nullptr && node->refCount > 0) ? node->address : nullptr;
}

template <typename T>
int LinkedList<T>::getRefCountById(int id) {
    Node* node = findById(id);
    if (node) {
        std::cout << "LinkedList::getRefCountById - ID: " << id << ", RefCount: " << node->refCount << std::endl;
        return node->refCount;
    }
    return 0;  // Ensure this doesn't incorrectly return 0 if the node should exist
}


template <typename T>
void LinkedList<T>::debug() const {
    Node* current = head;
    std::cout << "[LinkedList Debug] Estado de la lista enlazada:" << std::endl;
    while (current != nullptr) {
        std::cout << "  ID: " << current->id
                  << ", Ptr: " << current->address
                  << ", RefCount: " << current->refCount
                  << ", Valor: " << (current->address ? *(current->address) : 0) << std::endl;
        current = current->next;
    }
}


template <typename T>
LinkedList<T>::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        if (current->address != nullptr) {
            delete current->address;  // Libera la memoria apuntada
        }
        Node* temp = current;
        current = current->next;
        delete temp;  // Elimina el nodo
    }
}



#endif // LINKEDLIST_H
