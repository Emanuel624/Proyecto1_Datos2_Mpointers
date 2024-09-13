#include <gtest/gtest.h>
#include "MPointer.h"
#include "LinkedList.h"

///////////////////////////////////////////////////////LinkedList///////////////////////////////////////////////////////
// Caso de prueba: Insertar un nodo y verificar que su ID y dirección son correctos
TEST(LinkedListTest, InsertNode) {
    LinkedList<int> list;
    int* value = new int(42); // Valor a almacenar
    int id;
    list.insert(value, id);

    EXPECT_EQ(list.getCurrentId(), 1);  // Verificar que el ID es 1
    EXPECT_EQ(list.getAddressById(id), value);  // Verificar que la dirección coincide
    delete value; // Liberar la memoria asignada
}

// Caso de prueba: Insertar y luego buscar un nodo por dirección
TEST(LinkedListTest, FindByAddress) {
    LinkedList<int> list;
    int* value = new int(42);
    int id;
    list.insert(value, id);

    auto node = list.find(value);
    ASSERT_NE(node, nullptr);  // Verificar que se encuentra el nodo
    EXPECT_EQ(node->address, value);  // Verificar que la dirección coincide
    delete value;
}

// Caso de prueba: Insertar y luego buscar un nodo por ID
TEST(LinkedListTest, FindById) {
    LinkedList<int> list;
    int* value = new int(42);
    int id;
    list.insert(value, id);

    auto node = list.findById(id);
    ASSERT_NE(node, nullptr);  // Verificar que se encuentra el nodo
    EXPECT_EQ(node->id, id);  // Verificar que el ID coincide
    delete value;
}

// Caso de prueba: Verificar que el contador de referencias se actualiza correctamente
TEST(LinkedListTest, UpdateRefCount) {
    LinkedList<int> list;
    int* value = new int(42);
    int id;
    list.insert(value, id);

    list.setRefCountById(id, 5);  // Actualizar refCount a 5
    EXPECT_EQ(list.getRefCountById(id), 5);  // Verificar que el refCount es 5
    delete value;
}

// Caso de prueba: Eliminar un nodo por ID y verificar que ya no está en la lista
TEST(LinkedListTest, RemoveNode) {
    LinkedList<int> list;
    int* value = new int(42);
    int id;
    list.insert(value, id);

    list.remove(id);  // Eliminar el nodo
    EXPECT_EQ(list.getAddressById(id), nullptr);  // Verificar que ya no está en la lista
    delete value;
}

// Caso de prueba: Verificar que se maneja correctamente la destrucción de la lista
TEST(LinkedListTest, Destructor) {
    LinkedList<int>* list = new LinkedList<int>;
    int* value1 = new int(42);
    int* value2 = new int(84);
    int id1, id2;
    list->insert(value1, id1);
    list->insert(value2, id2);

    delete list;  // Destruir la lista
    // No se necesita liberar manualmente `value1` o `value2`, la lista solo elimina los nodo
}

// Caso de prueba: Buscar un nodo inexistente por dirección
TEST(LinkedListTest, FindNonExistentByAddress) {
    LinkedList<int> list;
    int* value = new int(42);
    int id;
    list.insert(value, id);

    int* fakeValue = new int(84);
    auto node = list.find(fakeValue);
    EXPECT_EQ(node, nullptr);  // Verificar que no se encuentra el nodo inexistente

    delete value;
    delete fakeValue;
}

///////////////////////////////////////////////////////MPointer/////////////////////////////////////////////////////////
// Caso de prueba: Crear un nuevo MPointer y verificar que el puntero no sea nulo
TEST(MPointerTest, CreateNewMPointer) {
    auto ptr = MPointer<int>::New();
    EXPECT_NE(ptr.get(), nullptr);  // Verifica que el puntero no sea nulo
}

// Caso de prueba: Verificar la asignación de valores con el operador *
TEST(MPointerTest, AssignValueUsingDereferenceOperator) {
    auto ptr = MPointer<int>::New();
    *ptr = 42;  // Asignar un valor
    EXPECT_EQ(*ptr, 42);  // Verificar que el valor es el esperado
}

// Caso de prueba: Verificar la sobrecarga del operador & para obtener el valor
TEST(MPointerTest, DereferenceAndGetValue) {
    auto ptr = MPointer<int>::New();
    *ptr = 100;  // Asignar un valor
    EXPECT_EQ(&ptr, 100);  // Verificar que & devuelve el valor correcto
}

// Caso de prueba: Verificar la asignación entre dos MPointers
TEST(MPointerTest, AssignmentBetweenMPointers) {
    auto ptr1 = MPointer<int>::New();
    *ptr1 = 50;

    auto ptr2 = MPointer<int>::New();
    *ptr2 = 75;

    ptr2 = ptr1;  // Asignar ptr1 a ptr2
    EXPECT_EQ(*ptr2, 50);  // Verificar que ptr2 tiene el mismo valor que ptr1

    MPointerGC<int>::getInstance()->debug();  // Usa el metodo debug para verificar el estado del GC
}

// Caso de prueba: Verificar la sobrecarga del operador = para valores primitivos
TEST(MPointerTest, AssignmentOperatorForPrimitiveValue) {
    auto ptr = MPointer<int>::New();
    *ptr = 20;  // Asignar un valor inicial
    ptr = 80;  // Sobrecarga del operador = para valores
    EXPECT_EQ(*ptr, 80);  // Verificar que el nuevo valor es el esperado
}

//Verificar el comportamiento del destructor (al reducir el refCount)
TEST(MPointerTest, DestructorBehavior) {
    auto ptr1 = MPointer<int>::New();
    *ptr1 = 42;

    {
        auto ptr2 = MPointer<int>::New();
        ptr2 = ptr1;  // ptr2 ahora apunta al mismo objeto que ptr1
    }  // ptr2 sale del alcance, se reduce el contador de referencias

    // Aquí debería quedar solo una referencia a ptr1
    EXPECT_EQ(*ptr1, 42);  // Verificar que el valor aún es accesible
}

//Verifica que se puedan crear puntero nulos
TEST(MPointerTest, NullPointerInitialization) {
    MPointer<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);  // Debe ser nulo
}

//Se revisa que el refCount si baje cuando se detruyen MPointers
TEST(MPointerTest, ReferenceCountDecreasesOnDestruction) {
    auto ptr1 = MPointer<int>::New();
    *ptr1 = 100;

    {
        auto ptr2 = ptr1;  // Incrementa el contador de referencias
        EXPECT_EQ(*ptr2, 100);  // Ptr2 tiene el mismo valor que ptr1
    }  // ptr2 sale del alcance, se reduce el contador de referencias (sin usar getRefCount)

    // Destruir ptr1, lo que debería liberar la memoria
    ptr1.~MPointer();
    MPointerGC<int>::getInstance()->debug();  // Verifica el estado del GC usando debug
}


///////////////////////////////////////////////////////MPointerGC///////////////////////////////////////////////////////
//Que el hilo si limpie la memoria correctamente
TEST(GarbageCollectorTest, GarbageCollectorFreesMemoryCorrectly) {
    auto ptr1 = MPointer<int>::New();
    *ptr1 = 42;

    {
        auto ptr2 = ptr1;  // Incrementa el contador de referencias
    }  // ptr2 sale del alcance, debería reducirse el contador de referencias

    ptr1.~MPointer();  // Destruir ptr1, RefCount debe llegar a 0

    std::this_thread::sleep_for(std::chrono::seconds(2));  // Esperar que el GC limpie la memoria
    MPointerGC<int>::getInstance()->debug();  // Verifica que el GC liberó la memoria correctamente
}

//Verificar funcionamiento del trhead como tal
TEST(GarbageCollectorTest, GarbageCollectorThreadRunning) {
    auto ptr1 = MPointer<int>::New();
    *ptr1 = 100;

    std::this_thread::sleep_for(std::chrono::seconds(1));  // Asegurar que el hilo del GC esté activo
    EXPECT_NE(MPointerGC<int>::getInstance(), nullptr);  // GC debe estar activo
}

//Main para hacer todas las pruebas a la vez
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
