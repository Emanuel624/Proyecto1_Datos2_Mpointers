#include <iostream>

// Definición de un nodo en la lista enlazada
struct Nodo {
    int dato;
    Nodo* siguiente;

    // Constructor para facilitar la creación de nodos
    Nodo(int valor) : dato(valor), siguiente(nullptr) {}
};

// Definición de la clase ListaEnlazada
class ListaEnlazada {
private:
    Nodo* cabeza;

public:
    // Constructor
    ListaEnlazada() : cabeza(nullptr) {}

    // Insertar un nuevo nodo al final de la lista
    void insertar(int valor) {
        Nodo* nuevoNodo = new Nodo(valor);
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
    }

    // Imprimir elementos de la lista
    void imprimir() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            std::cout << actual->dato << " -> ";
            actual = actual->siguiente;
        }
        std::cout << "null" << std::endl;
    }

    // Eliminar algun nodo especidico
    void eliminar(int valor) {
        if (cabeza == nullptr) return;

        if (cabeza->dato == valor) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return;
        }

        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->dato != valor) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != nullptr) {
            Nodo* temp = actual->siguiente;
            actual->siguiente = actual->siguiente->siguiente;
            delete temp;
        }
    }

    // Destructor para liberar memoria
    ~ListaEnlazada() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};

int main() {
    ListaEnlazada lista;
    lista.insertar(10);
    lista.insertar(20);
    lista.insertar(30);

    std::cout << "Lista después de insertar elementos: ";
    lista.imprimir();

    lista.eliminar(20);
    std::cout << "Lista después de eliminar el elemento 20: ";
    lista.imprimir();

    return 0;
}
