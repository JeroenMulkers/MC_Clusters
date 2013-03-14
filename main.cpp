#include <iostream>

int main (int argc, char* argv[]) {
    #ifdef DEBUGGING
    std::cout << "test" << std::endl;
    #endif
    return 0;
}
