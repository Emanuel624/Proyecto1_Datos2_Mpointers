#include <gtest/gtest.h> // Para poder usar GTEST
#include <sstream>  // Para std::stringstream
#include "LinkedList.h"
#include "MPointer.h"

//====================================================================================================================
// Pruebas para LinkedList

// Test para verificar la inserción en la lista enlazada
TEST(LinkedListTest, InsertAndFindTest) {
    LinkedList<int> list;
    int id;
    int* val = new int(10);

    list.insert(val, id);
    EXPECT_EQ(list.getAddressById(id), val);
    EXPECT_EQ(list.getRefCountById(id), 1);
}

// Test para verificar la eliminación en la lista enlazada
TEST(LinkedListTest, RemoveTest) {
    LinkedList<int> list;
    int id;
    int* val = new int(10);

    list.insert(val, id);
    list.decreaseRefCount(id);  // Asegura que RefCount llegue a 0 antes de eliminar
    list.remove(id);  // Ahora remove debería eliminar el nodo
    EXPECT_EQ(list.getAddressById(id), nullptr);
}

// Test para verificar la disminución del refCount
TEST(LinkedListTest, DecreaseRefCountTest) {
    LinkedList<int> list;
    int id;
    int* val = new int(10);

    list.insert(val, id);
    list.decreaseRefCount(id);
    EXPECT_EQ(list.getRefCountById(id), 0);
}

// Test para verificar la limpieza de la lista enlazada
TEST(LinkedListTest, CleanTest) {
    LinkedList<int> list;
    int id1, id2;
    int* val1 = new int(10);
    int* val2 = new int(20);

    list.insert(val1, id1);
    list.insert(val2, id2);
    list.decreaseRefCount(id1);  // Decrementa refCount para el primer nodo
    list.clean();  // Limpia nodos con refCount 0
    EXPECT_EQ(list.getAddressById(id1), nullptr);
    EXPECT_EQ(list.getAddressById(id2), val2);
}

// Test para verificar el destructor
TEST(LinkedListTest, DestructorTest) {
    LinkedList<int>* list = new LinkedList<int>();
    int id;
    int* val = new int(10);

    list->insert(val, id);
    delete list;  // Aquí se invoca al destructor
    // No hay assertion porque el destructor es automáticamente probado si no hay fugas de memoria.
}

// Test para verificar el findById
TEST(LinkedListTest, FindTest) {
    LinkedList<int> list;
    int* val1 = new int(10);
    int* val2 = new int(20);
    int id1, id2;

    list.insert(val1, id1);
    list.insert(val2, id2);

    // Prueba cuando se encuentra la dirección
    EXPECT_EQ(list.find(val1)->address, val1);

    // Prueba cuando no se encuentra la dirección
    int* notInList = new int(30);
    EXPECT_EQ(list.find(notInList), nullptr);

    delete notInList;
}

//======================================================================================================================
// Pruebas para MPointer

// Prueba de la función New()
TEST(MPointerTest, NewFunctionality) {
    auto ptr = MPointer<int>::New();
    EXPECT_NE(ptr.get(), nullptr);  // Verifica que el puntero subyacente no es nullptr
}

// Prueba del constructor por defecto
TEST(MPointerTest, DefaultConstructor) {
    MPointer<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);  // Verifica que el puntero subyacente es nullptr
}

// Prueba del constructor de copia
TEST(MPointerTest, CopyConstructor) {
    auto originalPtr = MPointer<int>::New();
    *originalPtr = 10;
    MPointer<int> copyPtr(originalPtr);

    EXPECT_EQ(copyPtr.operator*(), 10);
}

// Prueba de la sobrecarga del operador =
TEST(MPointerTest, AssignmentOperator) {
    auto ptr1 = MPointer<int>::New();
    auto ptr2 = MPointer<int>::New();

    *ptr1 = 20;
    ptr2 = ptr1;

    EXPECT_EQ(ptr2.operator*(), 20);
}

// Prueba de la sobrecarga del operador = con un valor de tipo T
TEST(MPointerTest, ValueAssignmentOperator) {
    auto ptr = MPointer<int>::New();
    ptr = 30;

    EXPECT_EQ(ptr.operator*(), 30);
}

// Prueba del operador de dereferencia
TEST(MPointerTest, DereferenceOperator) {
    auto ptr = MPointer<int>::New();
    *ptr = 40;

    EXPECT_EQ(*ptr, 40);
}

// Prueba del operador de referencia
TEST(MPointerTest, ReferenceOperator) {
    auto ptr = MPointer<int>::New();
    *ptr = 50;

    EXPECT_EQ(ptr.operator&(), 50);
}

// Prueba del destructor
TEST(MPointerTest, Destructor) {
    {
        auto ptr = MPointer<int>::New();
        *ptr = 60;
    }  // Destructor debería ser llamado aquí
    // Aquí podrías verificar si el Garbage Collector se comporta correctamente
}

//======================================================================================================================
// Pruebas para MPointerGC

//Falta prueba para ref(aumentar)

// Prueba de la función DecreaseRefCount en MPointerGC
TEST(MPointerGCTest, DecreaseRefCountFunction) {
    auto ptr = MPointer<int>::New();
    *ptr = 42;

    auto gc = MPointerGC<int>::getInstance();
    gc->DecreaseRefCount(ptr.operator&());

    int refCount = gc->getRefCountById(ptr.operator&());
    EXPECT_EQ(refCount, 0);  // El refCount debería ser 0 después de decrementar
}

// Prueba de la función Clean en MPointerGC
TEST(MPointerGCTest, CleanFunction) {
    {
        auto ptr = MPointer<int>::New();
        *ptr = 42;

        auto gc = MPointerGC<int>::getInstance();
        gc->DecreaseRefCount(ptr.operator&());  // Disminuye el refCount a 0
        gc->Clean();

        EXPECT_EQ(gc->getRefCountById(ptr.operator&()), 0);  // Verifica que el RefCount sea 0 después de la limpieza
    }  // Salida del alcance: el `ptr` debería ser limpiado

    std::this_thread::sleep_for(std::chrono::seconds(1));  // Pausa para permitir que el hilo de GC se ejecute
}

// Prueba del método debug
TEST(MPointerGCTest, DebugFunction) {
    auto ptr1 = MPointer<int>::New();
    *ptr1 = 100;

    auto ptr2 = MPointer<int>::New();
    *ptr2 = 200;

    auto gc = MPointerGC<int>::getInstance();
    gc->debug();  // Esto debería mostrar el estado actual del Garbage Collector

    SUCCEED();  // Si no se lanzan excepciones, la prueba pasa
}