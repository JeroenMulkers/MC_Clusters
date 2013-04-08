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

void initializeSet(set_particles &set, Ran1 &, double step);

int main (int argc, char* argv[]) {


    // default values
    int    seed     = 5;
    int    N        = 20;
    int    nMC      = 1000;
    int    ntest    = 1;
    double temp     = 0.;
    double step     = 0.2;
    double max_step = 1.;
    double min_step = 0.1;
    double factor   = 1.2;


    // read "input file"
    if( argc == 2 ){
        std::ifstream input(argv[1]);
        std::string str;
        std::string trash;
        while ( !input.eof() )
        {
            input >> str;
            if      ( str == "seed"     ) { input >> seed    ;}
            else if ( str == "N"        ) { input >> N       ;}
            else if ( str == "nMC"      ) { input >> nMC     ;}
            else if ( str == "temp"     ) { input >> temp    ;}
            else if ( str == "step"     ) { input >> step    ;}
            else if ( str == "max_step" ) { input >> max_step;}
            else if ( str == "min_step" ) { input >> min_step;}
            else if ( str == "ntest"    ) { input >> ntest   ;}
            else if ( str == "factor"   ) { input >> factor  ;}
            else                          { input >> trash ;}
        }
    }


    // Create random generator
    Ran1 RG(seed);


    // Start tests
    for(int test=0; test<ntest; test++){


        // Initialize the positions and calculate the initial energy
        set_particles set(N);
        initializeSet(set, RG, step);
        double energy = Energy(set);

        // Monte Carlo
        for(int iMC=0; iMC<nMC; iMC++){

            // Loop over all the particles
            for(auto & particle : set){

                // Back up the current positions and energy
                double x_backup = particle.x;
                double y_backup = particle.y;
                double energy_backup = energy;

                // Modify the position and caculate the energy and energy diff
                particle.x += particle.step * (2*RG.getNumber()-1);
                particle.y += particle.step * (2*RG.getNumber()-1);
                energy = Energy(set);
                double Ediff = energy - energy_backup;

                // Check if the new position is 'good' (energydiff low enough)
                if( Ediff < 0 || exp(-Ediff/temp) > RG.getNumber() ){
                    particle.step = std::min( particle.step*factor , max_step );
                } else {
                    energy = energy_backup;
                    particle.x = x_backup;
                    particle.y = y_backup;
                    particle.step = std::max( particle.step/factor , min_step );
                }
            }
        }


        // Prints the energy to the console
        std::cout << test << " final energy " << energy/N << std::endl;

        // Write coordinates to a file
        char filename[100];
        sprintf(filename, "coo_%d.dat", test);
        std::ofstream output(filename);
        for( auto p : set )
            output << p.x << "\t" << p.y << std::endl;
        output.close();

    }

    return 0;
}


void initializeSet(set_particles & set, Ran1 & RG, double step){
    int i = 0;

    // loop over every particle in the set
    for( auto & particle : set){

        double a = sqrt(0.25*(++i)*RG.getNumber());
        double rndm = RG.getNumber();

        particle.x = a*cos(2*PI*rndm);
        particle.y = a*sin(2*PI*rndm);
        particle.step = step;

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
