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

double energy(coordinates);

int main (int argc, char* argv[]) {

    int N = 10; // Number of particles

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

    std::cout << energy(coo) << std::endl;

    return 0;
}

double energy(coordinates coo){
    //Calculates the energy of a given configuration

    double energy = 0;

    for(auto it1 = coo.begin(); it1 != coo.end(); it1++){

        position p1 = *it1;
        energy += pow(p1.x,2);
        energy += pow(p1.y,2);

        for(auto it2 = it1+1; it2 != coo.end(); it2++){
            position p2 = *it2;
            energy += pow( pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2) , -0.5);
        }
    }

    return energy;
}
