#include <iostream>
#include "Ran1.h"

const int m1=259200, ia1=7141, ic1=54773;
const int m2=134456, ia2=8121, ic2=28411;
const int m3=243000, ia3=4561, ic3=51349;

const double rm1 = 1.0/m1;
const double rm2 = 1.0/m2;

Ran1::Ran1(int seed){
    setSeed(seed);
}

Ran1::~Ran1(){
}

void Ran1::setSeed(int seed){

    if(seed<0){
        std::cout << "The chosen seed is smaller than 0" << std::endl;
        std::cout << "The seed must be larger than 0" << std::endl;
        std::cout << "The default seed (0) will be used" << std::endl;
    }

    ix1=(ic1+seed) % m1;
    ix1=(ia1*ix1+ic1) % m1;
    ix2=ix1 % m2;
    ix1=(ia1*ix1+ic1) % m1;
    ix3=ix1 % m3;

    for(int i=0; i<97; i++){
        ix1=(ia1*ix1+ic1) % m1;
        ix2=(ia2*ix2+ic2) % m2;
        R[i]=(ix1+ix2*rm2)*rm1;
    }
}

double Ran1::getNumber(){

    ix1=(ia1*ix1+ic1) % m1;
    ix2=(ia2*ix2+ic2) % m2;
    ix3=(ia3*ix3+ic3) % m3;

    int j=(97*ix3)/m3;

    double random=0;

    if(j<97||j>=0){
        random=R[j];
        R[j]=(ix1+ix2*rm2)*rm1;
    }

    return random;
}
