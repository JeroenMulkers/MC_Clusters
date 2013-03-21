#include <iostream>
#include "Ran1.h"
#include <cmath>
#include <vector>
#include <tuple>

const double PI = 3.1415;

struct position{
    double x;
    double y;
};

typedef std::vector< position > coordinates;

int main (int argc, char* argv[]) {

    int N = 100; // Number of particles

    coordinates coo;
    coo.reserve(N);

    Ran1 RG(5);

    // Initial random numbers

    for(int i=0; i<N; i++){

        double a = sqrt(0.25*i*RG.getNumber());
        double rndm = RG.getNumber();

        position pos;
        pos.x = a*cos(2*PI*rndm);
        pos.y = a*sin(2*PI*rndm);

        coo.push_back(pos);
    }

    for(int i=0; i<N; i++){
        std::cout << coo[i].x << "\t" << coo[i].y << std::endl;
    }

    return 0;
}


