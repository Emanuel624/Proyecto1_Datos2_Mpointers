#include <iostream>
#include "MPointer.h"
int main() {
    MPointer<int> myPtr = MPointer<int>::New();
    myPtr = 5;
    int valor = &myPtr;

    std::cout<<valor<<std::endl;

    return 0;
}
