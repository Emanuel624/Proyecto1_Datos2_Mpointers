#ifndef MPOINTER_H
#define MPOINTER_H

#include "LinkedList.h"

// Declaración adelantada de la clase MPointerGC
template <typename T>
class MPointerGC;

template <typename T>
class MPointer {
private:
    T* ptr;
    int id;
    static MPointerGC<T>& gc; // Al ser estático, todas las instancias tienen el mismo GC

    // Hacer que MPointerGC sea una clase amiga
    friend class MPointerGC<T>;

public:
    // Para crear un objeto nuevo de forma estática
    static MPointer<T> New();

    // Constructor
    MPointer();

    // Constructor por copia (sobrecarga de constructor)
    MPointer(const MPointer<T>& other);

    // Operador de asignación (sobrecarga de =)
    MPointer<T>& operator=(const MPointer<T>& other);

    // Sobrecarga del operador de asignación para T
    MPointer<T>& operator=(const T& value);

    // Sobrecarga del operador *
    T& operator*();

    // Sobrecarga del operador &
    T* operator&();

    // Destructor
    ~MPointer();
};

// Clase GC
template <typename T>
class MPointerGC {
private:
    LinkedList<T> memoryList;

public:
    void Register(MPointer<T>& mpointer);

    void Unregister(int id);

    void Clean();

    ~MPointerGC();
};

// Definición del miembro estático `gc`
template <typename T>
MPointerGC<T>& MPointer<T>::gc = *(new MPointerGC<T>());

// Implementación de MPointer
template <typename T>
MPointer<T> MPointer<T>::New() {
    MPointer<T> newPtr;
    newPtr.ptr = new T();  // Reservar memoria para un nuevo objeto de tipo T
    gc.Register(newPtr);  // Registrar el nuevo puntero en el Garbage Collector
    return newPtr;
}

template <typename T>
MPointer<T>::MPointer() : ptr(nullptr), id(-1) {}

template <typename T>
MPointer<T>::MPointer(const MPointer<T>& other) {
    ptr = other.ptr;
    id = other.id;
    gc.Register(*this);
}

template <typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer<T>& other) {
    if (this != &other) {
        gc.Unregister(id);  // Eliminar la referencia anterior
        ptr = other.ptr;
        id = other.id;
        gc.Register(*this);
    }
    return *this;
}

template <typename T>
MPointer<T>& MPointer<T>::operator=(const T& value) {
    if (ptr) {
        *ptr = value;  // Asignar el valor al puntero interno
    }
    return *this;
}

template <typename T>
T& MPointer<T>::operator*() {
    return *ptr;
}

template <typename T>
T* MPointer<T>::operator&() {
    return ptr;
}

template <typename T>
MPointer<T>::~MPointer() {
    if (ptr != nullptr) {
        gc.Unregister(id);
        ptr = nullptr; // Evitar liberar la memoria dos veces
    }
}


// Implementación de MPointerGC
template <typename T>
void MPointerGC<T>::Register(MPointer<T>& mpointer) {
    auto node = memoryList.find(mpointer.ptr);
    if (node) {
        node->refCount++;
        mpointer.id = node->id;
    } else {
        memoryList.insert(mpointer.ptr, mpointer.id);
    }
}

template <typename T>
void MPointerGC<T>::Unregister(int id) {
    memoryList.remove(id);
}

template <typename T>
void MPointerGC<T>::Clean() {
    memoryList.clean();
}

template <typename T>
MPointerGC<T>::~MPointerGC() {
    Clean();
}

#endif // MPOINTER_H
