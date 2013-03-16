#include <iostream>
#include "Ran1.h"

int main (int argc, char* argv[]) {
    Ran1 random(-5);
    for(int i=0; i<100; i++){
        std::cout << random.getNumber() << std::endl;
    }
    return 0;
}
