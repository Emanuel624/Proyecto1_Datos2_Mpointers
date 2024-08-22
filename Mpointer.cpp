#include "MPointer.h"
#include <iostream>

template <typename T>
MPointerGC<T>& MPointer<T>::gc = *(new MPointerGC<T>());

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
T& MPointer<T>::operator*() {
    return *ptr;
}

template <typename T>
T* MPointer<T>::operator&() {
    return ptr;
}

template <typename T>
MPointer<T>::~MPointer() {
    gc.Unregister(id);
    delete ptr;  // Liberar la memoria reservada
}

template <typename T>
void MPointerGC<T>::Register(MPointer<T>& mpointer) {
    static int currentId = 0;
    mpointer.id = ++currentId;
    memoryMap[mpointer.id] = mpointer.ptr;
}

template <typename T>
void MPointerGC<T>::Unregister(int id) {
    memoryMap.erase(id);
}

template <typename T>
void MPointerGC<T>::Clean() {
    for (auto it = memoryMap.begin(); it != memoryMap.end(); ) {
        if (/* Condición para liberar memoria */) {
            delete it->second;
            it = memoryMap.erase(it);
        } else {
            ++it;
        }
    }
}

