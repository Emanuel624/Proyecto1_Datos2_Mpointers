#ifndef MPOINTER_H
#define MPOINTER_H

#include "LinkedList.h"  // Incluye la lista enlazada que manejas
#include <thread>
#include <mutex>
#include <chrono>
#include <type_traits>  // Incluir la biblioteca para std::is_same

template <typename T>
class MPointerGC;

// Clase MPointer
template <typename T>
class MPointer {
private:
    T* ptr;  // Puntero de tipo T*
    int id;  // ID único para cada MPointer
    static MPointerGC<T>* gc;  // Puntero al Garbage Collector

    friend class MPointerGC<T>;  // MPointerGC tiene acceso a los miembros privados

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

// Implementación
template <typename T>
MPointerGC<T>* MPointer<T>::gc = MPointerGC<T>::getInstance();

// Crear un MPointer con el metodo New()
template <typename T>
MPointer<T> MPointer<T>::New() {
    MPointer<T> newPtr;
    newPtr.ptr = new T();  // Asigna memoria para T en el heap
    gc->Register(newPtr);  // Registra el nuevo MPointer en el GC
    return newPtr;  // Retorna el nuevo MPointer
}

template <typename T>
MPointer<T>::MPointer() : ptr(nullptr), id(-1) {}

template <typename T>
MPointer<T>::MPointer(const MPointer<T>& other) {
    ptr = other.ptr;
    id = other.id;
    gc->Register(*this);  // Registra la copia en el GC
}

template <typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer<T>& other) {
    if (this != &other) {
        gc->Unregister(id);  // Desregistra el ID actual de myPtr2
        ptr = other.ptr;      // Copia la dirección de memoria de myPtr
        id = other.id;        // Copia el ID de myPtr a myPtr2
        gc->Register(*this);  // Registra la nueva asignación
    }
    return *this;
}

template <typename T>
MPointer<T>& MPointer<T>::operator=(const T& value) {
    if (ptr) {
        *ptr = value;  // Asigna el nuevo valor al objeto apuntado
    }
    return *this;
}

template <typename T>
T& MPointer<T>::operator*() {
    return *ptr;  // Devuelve una referencia al objeto apuntado
}

template <typename T>
T MPointer<T>::operator&() {
    return *ptr;  // Devuelve el valor al que apunta ptr
}

template <typename T>
MPointer<T>::~MPointer() {
    std::cout << "Destructor llamado para MPointer con id: " << id << std::endl;
    if (ptr != nullptr) {
        delete ptr;  // Libera la memoria apuntada por ptr
        gc->Unregister(id);  // Desregistra el MPointer del GC
    }
}

// Clase GC
template <typename T>
class MPointerGC {
private:
    LinkedList<T> memoryList;  // Lista enlazada que guarda direcciones de memoria
    static MPointerGC<T>* instance;  // Singleton para la instancia de GC
    static std::mutex gcMutex;  // Mutex para sincronización

    MPointerGC() = default;

    void GC_CleanupThread(int interval) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(interval));
            Clean();  // Limpia la memoria no utilizada
        }
    }

public:
    static MPointerGC<T>* getInstance() {
        std::lock_guard<std::mutex> lock(gcMutex);
        if (instance == nullptr) {
            instance = new MPointerGC<T>();
            std::thread(&MPointerGC<T>::GC_CleanupThread, instance, 5).detach();  // Hilo separado para la limpieza periódica
        }
        return instance;
    }

    void Register(MPointer<T>& mpointer);
    void Unregister(int id);
    void Clean();
    ~MPointerGC() {
        Clean();  // Limpia la memoria al destruir el GC
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
        node->refCount++;  // Incrementa el contador de referencias si ya existe
        mpointer.id = node->id;  // Copia el ID
    } else {
        memoryList.insert(mpointer.ptr, mpointer.id);  // Inserta la nueva dirección y genera un nuevo ID
    }
}

template <typename T>
void MPointerGC<T>::Unregister(int id) {
    memoryList.remove(id);  // Desregistra y reduce el contador de referencias
}

template <typename T>
void MPointerGC<T>::Clean() {
    memoryList.clean();  // Elimina nodos cuya referencia es cero
}

#endif  // MPOINTER_H