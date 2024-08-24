#ifndef MPOINTER_H
#define MPOINTER_H

#include "LinkedList.h"
#include <thread>
#include <mutex>
#include <chrono>

template <typename T>
class MPointerGC;

// Clase MPointer
template <typename T>
class MPointer {
private:
    T* ptr; //Parametro de tipo T*
    int id; //Id para cada MPointer
    static MPointerGC<T>* gc; //GC para manejo de memoria de la clase

    //Se hace "clase amiga" al GC
    friend class MPointerGC<T>;

public:
    static MPointer<T> New();
    MPointer();
    MPointer(const MPointer<T>& other);
    MPointer<T>& operator=(const MPointer<T>& other);
    MPointer<T>& operator=(const T& value);
    T& operator*();
    T operator&();
    ~MPointer();
};

//Implementación
template <typename T>
MPointerGC<T>* MPointer<T>::gc = MPointerGC<T>::getInstance();

template <typename T>
MPointer<T> MPointer<T>::New() {
    MPointer<T> newPtr;
    newPtr.ptr = new T(); //Se asigna memoria para el parametro T (en el heap)
    gc->Register(newPtr); //Se registra en el GC
    return newPtr; //Se retorna el MPointer
}

template <typename T>
MPointer<T>::MPointer() : ptr(nullptr), id(-1) {}

template <typename T>
MPointer<T>::MPointer(const MPointer<T>& other) {
    ptr = other.ptr;
    id = other.id;
    gc->Register(*this);
}

//En caso de que en el operador de asignacion ambos sean de Mpointer
template <typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer<T>& other) {
    if (this != &other) {
        gc->Unregister(id); //Se desregistra el lugar a donde estaba apuntando el otro MPointer antes de la asiganacion
        ptr = other.ptr; //Se apunta a la misma direccion de memoria
        id = other.id; //Se copia el IDS
        gc->Register(*this);
    }
    return *this;
}

template <typename T>
MPointer<T>& MPointer<T>::operator=(const T& value) {
    if (ptr) {
        *ptr = value;
    }
    return *this;
}

template <typename T>
T& MPointer<T>::operator*() {
    return *ptr;
}

// Sobrecarga del operador & para devolver el valor al que apunta ptr
template <typename T>
T MPointer<T>::operator&() {
    return *ptr;
}


template <typename T>
MPointer<T>::~MPointer() {
    if (ptr != nullptr) {
        gc->Unregister(id);
    }
}

// Clase GC
template <typename T>
class MPointerGC {
private:
    LinkedList<T> memoryList;
    static MPointerGC<T>* instance;
    static std::mutex gcMutex;

    MPointerGC() = default;

    void GC_CleanupThread(int interval) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(interval));
            Clean();
        }
    }

public:
    static MPointerGC<T>* getInstance() {
        std::lock_guard<std::mutex> lock(gcMutex);
        if (instance == nullptr) {
            instance = new MPointerGC<T>();
            std::thread(&MPointerGC<T>::GC_CleanupThread, instance, 5).detach();
        }
        return instance;
    }

    void Register(MPointer<T>& mpointer);
    void Unregister(int id);
    void Clean();
    ~MPointerGC() {
        Clean();
    }
};

template <typename T>
MPointerGC<T>* MPointerGC<T>::instance = nullptr;

template <typename T>
std::mutex MPointerGC<T>::gcMutex;

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

#endif // MPOINTER_H
