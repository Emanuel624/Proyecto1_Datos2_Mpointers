#include <iostream>
#include "MPointer.h"
#include "../../../../usr/include/complex.h"

MPointer<int> foo() {
    MPointer<int> temp2 = MPointer<int>::New();
    MPointer<int> temp = MPointer<int>::New();
    temp = 666;
    MPointerGC<int>::getInstance()->debug();
    return temp;
}


int main() {
    {
        MPointer<int> mp1 = MPointer<int>::New();
        MPointer<int> mp2 = MPointer<int>::New();
        foo();

        MPointerGC<int>::getInstance()->debug();


        *mp1 = 100;
        mp2 = 50;


        MPointer<int> mp3 = mp2;
        MPointerGC<int>::getInstance()->debug();


        mp3 = mp1;
        MPointerGC<int>::getInstance()->debug();
        MPointerGC<int>::getInstance()->debug();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));  // Pausa para lograr que el thread de GC se ejecute

    return 0;
}

