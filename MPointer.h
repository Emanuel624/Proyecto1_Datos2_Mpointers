#ifndef MPOINTER_H
#define MPOINTER_H

#include "LinkedList.h"  // Incluye la lista enlazada
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

template <typename T>
class MPointerGC;

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
        return ptr; //Poder retornar un atributo privado
    }

    int getId() {
        return id; //Poder retornar un atributo privado
    }

    // Para shallow copy
    MPointer(const MPointer<T>& other);

    // Sobrecarga del operador = para asignación de otro MPointer
    MPointer<T>& operator=(const MPointer<T>& other);

    // Sobrecarga del operador = para nullptr (Lista doblemente enlazada)
    MPointer<T>& operator=(std::nullptr_t) {
        if (ptr != nullptr) {
            gc->DecreaseRefCount(id);  // Reduce el contador de referencias
            ptr = nullptr;  // Asigna nullptr
            id = -1;        // Reinicia el ID
        }
        return *this;
    }

    // Sobrecarga del operador = para asignación de un valor de tipo T (el mismo a lo interno de MPointer)
    template <typename U,
              typename = typename std::enable_if<std::is_same_v<T, U>>::type>
    MPointer<T>& operator=(const U& value) {
        if (ptr) {
            *ptr = value;  // Asigna el nuevo valor al objeto apuntado
        }
        return *this;
    }

    // Constructor que acepta nullptr
    MPointer(std::nullptr_t) : ptr(nullptr), id(-1) {}


    // Sobrecarga del operador != para nullptr (para lista doblemente enlazada)
    bool operator!=(std::nullptr_t) const {
        return ptr != nullptr;
    }

    // Sobrecarga del operador -> (para lista doblemente enlazada)
    T* operator->() const {
        return ptr;
    }

    // Sobrecarga del operador == para nullptr (para lista doblemente enlazada)
    bool operator==(std::nullptr_t) const {
        return ptr == nullptr;
    }


    T& operator*();
    T operator&();
    ~MPointer();
};


// Inicializa la clase singleton de MPointerGC
template <typename T>
MPointerGC<T>* MPointer<T>::gc = MPointerGC<T>::getInstance();

// Metodo para crear un nuevo MPointer y guardar el espacio para el dato por guardar
template <typename T>
MPointer<T> MPointer<T>::New() {
    MPointer<T> newPtr;
    newPtr.ptr = new T();  // Asigna memoria para T en el heap
    gc->Register(newPtr);  // Registra el nuevo MPointer en el GC
    return newPtr;  // Retorna el nuevo MPointer
}

//Constructor por default (no funciona, para que sea por el metodo new)
template <typename T>
MPointer<T>::MPointer() : ptr(nullptr), id(-1) {}

// Shallow Copy
template <typename T>
MPointer<T>::MPointer(const MPointer<T>& other) {
    ptr = other.ptr;
    id = other.id;
    gc->Register(*this);  // Registra la copia en el GC
}

// Sobrecarga del operador "=" en caso de que sean 2 de tipo MPointer
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

// Sobrecarga del operador * para almacenar el puntero
template <typename T>
T& MPointer<T>::operator*() {
    return *ptr;  // Devuelve una referencia al objeto apuntado
}

// Sobrecarga del operador & para obtener el valor guardado
template <typename T>
T MPointer<T>::operator&() {
    return *ptr;  // Devuelve el valor al que apunta ptr
}

// Destructor de MPointer que llama a MPointerGC
template <typename T>
MPointer<T>::~MPointer() {
    gc->DecreaseRefCount(id);  // Informa al GC para disminuir el contador de referencias
}


// Clase GC
template <typename T>
class MPointerGC {
private:
    LinkedList<T> memoryList;  // Lista enlazada que guarda direcciones de memoria
    static MPointerGC<T>* instance;  // Singleton para la instancia de GC
    static std::mutex gcMutex;  // Mutex para sincronización del thread
    std::thread gcThread;  // Hilo para ejecutar la limpieza periódica
    bool running = true;  // Controla si el hilo de limpieza sigue ejecutándose

    // Metodo que se ejecuta en el hilo cada 1 segundo para limpiar la memoria
    void GC_CleanupThread() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));  // Espera de 1 segundo
            std::lock_guard<std::mutex> lock(gcMutex);
            std::cout << "[GC Thread] Revisando referencias..." << std::endl;

            // Recorre los nodos de la lista enlazada y libera aquellos con refCount 0
            for (int id = 1; id <= memoryList.getCurrentId(); ++id) {
                int refCount = memoryList.getRefCountById(id);
                if (refCount == 0) {
                    //std::cout << "[GC Thread] refCount 0 for ID: " << id << std::endl;
                    FreeMemory(id);  // Libera la memoria
                }
            }
        }
    }


    // Constructor que inicia el hilo de limpieza
    MPointerGC() {
        gcThread = std::thread(&MPointerGC::GC_CleanupThread, this);
    }

public:
    //Metodo estatica que vuelve a la clase Singleton
    static MPointerGC<T>* getInstance() {
        std::lock_guard<std::mutex> lock(gcMutex);
        if (instance == nullptr) {
            instance = new MPointerGC<T>();
        }
        return instance;
    }

    // Metodo de depuracion
    void debug() {
        std::lock_guard<std::mutex> lock(gcMutex);
        std::cout << "[GC Debug] Estado actual de la memoria:" << std::endl;

        for (int id = 1; id <= memoryList.getCurrentId(); ++id) {
            T* address = memoryList.getAddressById(id);
            int refCount = memoryList.getRefCountById(id);
            if (address != nullptr) {
                std::cout << "ID: " << id
                          << ", Dirección de Memoria: " << address
                          << ", Valor: " << *address
                          << ", RefCount: " << refCount
                          << std::endl;
            }
        }
    }

    //Obtener el refCount de un nodo especifico (es decir de un MPointer)
    int getRefCount(int id) const {
        return memoryList.getRefCountById(id);
    }

    //Obtener la dirrecion de memoria guardada dentro de la lista enlazada
    T* getAddress(int id) const {
        return memoryList.getAddressById(id);
    }

    // Registrar un nuevo MPointer
    void Register(MPointer<T>& mpointer);

    // Incrementar el contador de referencias
    void IncreaseRefCount(int id);

    // Decrementar el contador de referencias
    void DecreaseRefCount(int id);

    // Liberar memoria cuando el refCount llega a cero
    void FreeMemory(int id);

    // Destructor (detiene el hilo y libera la memoria restante)
    ~MPointerGC();
};

template <typename T>
MPointerGC<T>* MPointerGC<T>::instance = nullptr;

template <typename T>
std::mutex MPointerGC<T>::gcMutex;

//Registro dentro del GC
template <typename T>
void MPointerGC<T>::Register(MPointer<T>& mpointer) {
    auto node = memoryList.find(mpointer.ptr);
    if (node) {
        IncreaseRefCount(node->id);  // Incrementa el refCount si ya existe
        mpointer.id = node->id;  // Asigna el ID existente al MPointer
    } else {
        int newId;
        memoryList.insert(mpointer.ptr, newId);  // Inserta la nueva dirección y genera un nuevo ID
        mpointer.id = newId;  // Asigna el nuevo ID al MPointer
    }
}

//Aumnetar el refCount
template <typename T>
void MPointerGC<T>::IncreaseRefCount(int id) {
    int refCount = memoryList.getRefCountById(id);
    memoryList.setRefCountById(id, refCount + 1);  // Incrementa el refCount
}

//Disminuir el refCount
template <typename T>
void MPointerGC<T>::DecreaseRefCount(int id) {
    int refCount = memoryList.getRefCountById(id);
    if (refCount > 0) {
        memoryList.setRefCountById(id, refCount - 1);  // Decrementa el refCount
        //std::cout << "Decreasing refCount for ID: " << id << " to " << refCount - 1 << std::endl;
    }
}

//Libera la memoria del puntero interno
template <typename T>
void MPointerGC<T>::FreeMemory(int id) {
    T* address = memoryList.getAddressById(id);
    if (address) {
        std::cout << "Liberando memoria para ID: " << id << std::endl;
        delete address;  // Libera la memoria asignada
    }
    memoryList.remove(id);  // Elimina el nodo de la lista de memoria
    //std::cout << "Memoria para ID: " << id << " fue liberada y removida de la lista" << std::endl;
}


//Destructor, en caso de que el Thread no haya limpiado la memoria y el programa pare
template <typename T>
MPointerGC<T>::~MPointerGC() {
    running = false;  // Detiene el hilo
    if (gcThread.joinable()) {
        gcThread.join();  // Espera a que el hilo termine
    }

    std::cout << "Liberando todos los recursos en MPointerGC destructor." << std::endl;

    // Limpia toda la memoria restante si no fue liberada previamente
    for (int id = 1; id <= memoryList.getCurrentId(); ++id) {
        int refCount = memoryList.getRefCountById(id);
        if (refCount == 0) {
            FreeMemory(id);  // Libera la memoria para cualquier ID que aún no haya sido liberado
        }
    }
}

#endif  // MPOINTER_H
