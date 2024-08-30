cmake_minimum_required(VERSION 3.28)
project(Proyecto1_Datos2_Mpointers)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proyecto1_Datos2_Mpointers main.cpp)

# Especifica que estamos creando una biblioteca estática
add_library(Mpointers STATIC MPointer.cpp)

# Incluye el directorio actual para buscar los archivos de cabecera
target_include_directories(Mpointers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})