#include <iostream>
#include "Ran1.h"

int main (int argc, char* argv[]) {

    Ran1 RG(5);

    for(int i=0; i<10; i++){
        std::cout << RG.getNumber() << std::endl;
    }

    return 0;
}
