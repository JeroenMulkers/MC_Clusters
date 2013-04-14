#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>
#include <iomanip>

#include "Ran1.h"
#include "Particles.h"


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
    double yukawa   = 0;


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
            else if ( str == "yukawa"   ) { input >> yukawa  ;}
            else                          { input >> trash   ;}
        }
    }


    // Create random generator
    Ran1 RG(seed);


    // Start tests
    for(int test=0; test<ntest; test++){


        // Initialize the positions and calculate the initial energy
        Set_particles set(N);
        initializeSet(set, RG, step);
        double energy = Energy(set,yukawa);

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
                energy = Energy(set,yukawa);
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
        std::cout << "test " << test << " final energy " << energy/N << std::endl;

        // Write results of the test to a file
        char filename[20];
        sprintf(filename, "coo_%d.dat", test);
        std::ofstream output(filename);
        output << "# Test " << test << std::endl;
        output << "# Number of particles " << N << std::endl;
        output << "# Energy per particle " << energy/N << std::endl;
        output << "#==============================" << std::endl;
        output << "# x\ty\tr" << std::endl;
        PrintCoordinates(set,output);
        output.close();

    }

    return 0;
}


