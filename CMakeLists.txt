cmake_minimum_required(VERSION 3.28)
project(Proyecto1_Datos2_Mpointers)

set(CMAKE_CXX_STANDARD 17)

# Especifica el ejecutable
add_executable(Proyecto1_Datos2_Mpointers main.cpp)

# Especifica que se crea una biblioteca estática
add_library(Mpointers STATIC MPointer.cpp)

# Incluye el directorio actual para buscar los archivos de cabecera
target_include_directories(Mpointers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Enlace a la biblioteca GTest
find_package(GTest REQUIRED)
enable_testing()

# Agrega el archivo de prueba
add_executable(test_mpointer test_mpointer.cpp)
target_link_libraries(test_mpointer GTest::GTest GTest::Main Mpointers)
add_test(NAME test_mpointer COMMAND test_mpointer)
