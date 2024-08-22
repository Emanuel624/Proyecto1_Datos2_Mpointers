#ifndef MPOINTER_H
#define MPOINTER_H

#include <iostream>
#include <unordered_map>
#include <memory>

// Declaración adelantada de la clase MPointerGC (para poder usar en clase MPointer)
template <typename T>
class MPointerGC;


//Clase Mpointer
template <typename T>
class MPointer {
private:
    T* ptr;
    int id;
    static MPointerGC<T> &gc; //Al ser estatico todas las instancias tienen el mismo


public:
    //Para crear un objeto nuevo de forma estatica
    static MPointer<T> New();

    //Constructor como tal
    MPointer();

    // Constructor por copia (sobrecarga de constructor)
    MPointer(const MPointer<T>& other);

    // Operador de asignación (sobrecarga de =)
    MPointer<T>& operator=(const MPointer<T>& other);

    //Sobrecarga del operador *
    T& operator*();

    //Sobrecarga del operador &
    T* operator&();


    //Destructor
    ~MPointer();
};


//Clase GC
template <typename  T>
class MPointerGC {
private:
    std::unordered_map<int, T*> memoryMap;

public:
    void Register(MPointer<T>& mpointer);

    void Unregister(int id);

    void Clean();
};

#endif //MPOINTER_H
