#include <iostream>
#include <fstream>
#include "Ran1.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>

const double PI = 3.14159265359;

struct particle{
    double x;
    double y;
    double step;
};

typedef std::vector< particle > set_particles;

double Energy(set_particles);

void initializeSet(set_particles &set, Ran1 &);

/********************************************************/

int main (int argc, char* argv[]) {

    // default values

    int seed = 5;
    int N = 20;
    int nMC = 1000;
    double temp = 0;
    double step = 0.2;
    double max_step = 1;
    double min_step = 0.1;
    double factor = 1.2;

    // read "input file"

    if( argc == 2 ){
        std::ifstream input(argv[1]);
        std::string str;
        std::string trash;
        while ( !input.eof() )
        {
            input >> str;
            if      ( str == "seed"      ) { input >> seed    ;}
            else if ( str == "N"         ) { input >> N       ;}
            else if ( str == "nMC"       ) { input >> nMC     ;}
            else if ( str == "temp"      ) { input >> temp    ;}
            else if ( str == "step"      ) { input >> step    ;}
            else if ( str == "max_step"  ) { input >> max_step;}
            else if ( str == "min_step"  ) { input >> min_step;}
            else                       { input >> trash ;}
        }
    }

    Ran1 RG(seed);

    set_particles set(N);
    initializeSet(set, RG);

    double energy = Energy(set);

    for(int iMC=0; iMC<nMC; iMC++){

        for(auto & p : set){

            double x_backup = p.x;
            double y_backup = p.y;

            p.x += p.step*(2*RG.getNumber()-1);
            p.y += p.step*(2*RG.getNumber()-1);

            double newEnergy = Energy(set);
            double Ediff = newEnergy - energy;

            if( Ediff < 0 || exp(-Ediff/temp) > RG.getNumber() ){
                energy = newEnergy;
                p.step = std::min(p.step*factor,max_step);
            } else {
                p.x = x_backup;
                p.y = y_backup;
                p.step = std::max(p.step/factor,min_step);
            }
        }
    }

    for( auto p : set ){
        std::cout << p.x << "\t" << p.y << std::endl;
    }

    return 0;
}

/********************************************************/

void initializeSet(set_particles & set, Ran1 & RG, double step){
    int i = 0;
    for( auto & p : set){

        double a = sqrt(0.25*(++i)*RG.getNumber());
        double rndm = RG.getNumber();

        p.x = a*cos(2*PI*rndm);
        p.y = a*sin(2*PI*rndm);
        p.step = step;

    }
}

double Energy(set_particles set){
    //Calculates the energy of a given configuration

    double energy = 0;

    for(auto it1 = set.begin(); it1 != set.end(); it1++){

        particle p1 = *it1;
        energy += pow(p1.x,2);
        energy += pow(p1.y,2);

        for(auto it2 = it1+1; it2 != set.end(); it2++){
            particle p2 = *it2;
            energy += pow( pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2) , -0.5);
        }
    }

    return energy;
}
