#ifndef LINKEDLIST_H
#define LINKEDLIST_H

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
    void clean();
    ~LinkedList();
};

// Implementación de LinkedList
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
            current->refCount--;
            if (current->refCount == 0) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
                }
                delete current; // No se elimina 'address' aquí, ya que lo hace el destructor de MPointer
            }
            return;
        }
        previous = current;
        current = current->next;
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
            Node* temp = current;
            current = current->next;
            delete temp;  // Solo se elimina el nodo, no la memoria apuntada por 'address'
        } else {
            previous = current;
            current = current->next;
        }
    }
}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

#endif // LINKEDLIST_H