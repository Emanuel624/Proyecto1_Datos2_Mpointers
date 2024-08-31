#ifndef MPOINTER_H
#define MPOINTER_H

#include "LinkedList.h"  // Incluye la lista enlazada
#include <thread>
#include <mutex>
#include <chrono>
#include <type_traits>  // Necesario para std::enable_if y std::is_same_v>
#include <iostream>

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

    T* get() const {
        return ptr;
    }

    //Para shallow copy
    MPointer(const MPointer<T>& other);

    // Sobrecarga del operador = para asignación de otro MPointer
    MPointer<T>& operator=(const MPointer<T>& other);

    // Sobrecarga del operador = para asignación de un valor de tipo T
    template <typename U,
              typename = typename std::enable_if<std::is_same_v<T, U>>::type>
    MPointer<T>& operator=(const U& value) {
        if (ptr) {
            *ptr = value;  // Asigna el nuevo valor al objeto apuntado
        }
        return *this;
    }

    T& operator*();
    T operator&();
    ~MPointer();
};

// Implementación
template <typename T>
MPointerGC<T>* MPointer<T>::gc = MPointerGC<T>::getInstance();

template <typename T>
MPointer<T> MPointer<T>::New() {
    MPointer<T> newPtr;
    newPtr.ptr = new T();  // Asigna memoria para T en el heap
    std::cout << "MPointer::New() - Address: " << newPtr.ptr << std::endl;
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
        gc->DecreaseRefCount(id);  // Reduce el contador de referencias
        ptr = other.ptr;           // Copia la dirección de memoria
        id = other.id;             // Copia el ID
        gc->Register(*this);       // Registra la nueva asignación
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
    //std::cout << "Destructor llamado para MPointer con id: " << id << std::endl;
    gc->DecreaseRefCount(id);  // Informa al GC para disminuir el contador de referencias
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
            std::cout << "[GC Thread] Ejecutando limpieza..." << std::endl;
            Clean();  // Limpia la memoria no utilizada
            std::cout << "[GC Thread] Limpieza completada." << std::endl;
        }
    }

public:
    static MPointerGC<T>* getInstance() {
        std::lock_guard<std::mutex> lock(gcMutex);
        if (instance == nullptr) {
            instance = new MPointerGC<T>();
            //std::cout << "[GC Thread] Iniciando thread de GC..." << std::endl;
            std::thread(&MPointerGC<T>::GC_CleanupThread, instance, 1).detach();  // Hilo separado para la limpieza periódica
        }
        return instance;
    }

    void Register(MPointer<T>& mpointer);
    void DecreaseRefCount(int id);
    int getRefCountById(int id);  // Se obtiene la cantidad de referencias a una memoria
    void Clean();
    ~MPointerGC();

    // Nueva función debug para imprimir el estado de todos los MPointer registrados
    void debug() const {
        //std::cout << "[GC Debug] Estado actual del Garbage Collector:" << std::endl;
        memoryList.debug();
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
        mpointer.id = node->id;  // Asigna el ID existente al MPointer
        std::cout << "MPointerGC::Register() - Existing Node Found. RefCount incrementado a: " << node->refCount << std::endl;
    } else {
        int newId;
        memoryList.insert(mpointer.ptr, newId);  // Inserta la nueva dirección y genera un nuevo ID
        mpointer.id = newId;  // Asigna el nuevo ID al MPointer
        std::cout << "MPointerGC::Register() - New Node Created. ID: " << newId << ", RefCount: " << memoryList.getRefCountById(newId) << std::endl;
    }
}

template <typename T>
void MPointerGC<T>::DecreaseRefCount(int id) {
    auto node = memoryList.findById(id);
    if (node && node->refCount > 0) {
        node->refCount--;  // Decrementa el contador de referencias
        std::cout << "MPointerGC::DecreaseRefCount() - ID: " << id << ", RefCount decrementado a: " << node->refCount << std::endl;
        if (node->refCount == 0) {
            memoryList.remove(id);  // Si llega a 0, elimina el nodo
        }
    }
}

template <typename T>
int MPointerGC<T>::getRefCountById(int id) {
    return memoryList.getRefCountById(id);  // Llama a la función para obtener el refCount
}

template <typename T>
void MPointerGC<T>::Clean() {
    memoryList.clean();  // Elimina nodos cuya referencia es cero y libera la memoria asociada
}


template <typename T>
MPointerGC<T>::~MPointerGC() {
    Clean();  // Limpia la memoria al destruir el GC
}

#endif  // MPOINTER_H