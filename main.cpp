#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iomanip>
#include "omp.h"

#include "Ran1.h"
#include "Particles.h"


int main (int argc, char* argv[]) {


    // Remove old data files
    system("rm -f *.energy test_*.coo");


    // default values
    int    seed     = 5;
    int    N        = 20;
    int    nMC      = 1000;
    int    ntest    = 1;
    bool   verbose  = 0;
    double temp     = 0.;
    double step     = 0.2;
    double max_step = 1.;
    double min_step = 0.1;
    double factor   = 1.2;
    double yukawa   = 0;
    std::string ini_coo = "random";


    // read "input file"
    if( argc == 2 ){
        std::ifstream input(argv[1]);
        std::string str;
        std::string trash;
        while ( !input.eof() ){
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
            else if ( str == "ini_coo"  ) { input >> ini_coo ;}
            else if ( str == "verbose"  ) { input >> verbose ;}
            else                          { input >> trash   ;}
        }
        input.close();
    }


    // Create random generator
    Ran1 RG(seed);


    // Vector to put the results in from the different tests
    std::vector<double> energies;


		// Number of Metropolis successes and failures
		int ps = 0;
		int pf = 0;


    // begin tests
    for(int test=0; test<ntest; test++){


        // Initialize the positions and calculate the initial energy
        Set_particles set(N);
        initializeSet( set, RG, step, ini_coo);
        double energy = Energy(set,yukawa);
        if(verbose){ PrintCoordinates(set, energy, test, 0, 'i'); }


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
										ps++;
                } else {
                    energy = energy_backup;
                    particle.x = x_backup;
                    particle.y = y_backup;
                    particle.step = std::max( particle.step/factor , min_step );
										pf++;
                }
            }


            // Print the temporary result to a file
            if( verbose ){
                char filename[25];
                sprintf(filename, "test_%d.energy",test);
                std::ofstream output_energy(filename,std::ofstream::app);
                output_energy << std::setprecision(9) << energy/N << std::endl;
                output_energy.close();
                PrintCoordinates(set, energy, test, iMC, 's');
            }

        }


        // Save the resulting energy of current test
        energies.push_back(energy/N);


        // Print the final result
        PrintCoordinates(set, energy, test, nMC, 'f');
    }


    // Find lowest energy
    double lowest_energy = energies[0];
    int    lowest_test = 0;
    char   lowest_coo[25];
    for(int i=0; i<ntest; i++){
        if (energies[i] < lowest_energy){
            lowest_test = i;
            lowest_energy = energies[i];
        }
    }
    sprintf(lowest_coo,"test_%d_final.coo",lowest_test);


    // Print output
    std::cout << std::endl << std::endl;
    std::cout << "=INPUT=PARAMETERS===========================";
    std::cout << std::endl << std::endl;
    std::cout << "  input file          " << argv[1]  << std::endl;
    std::cout << "  seed                " << seed     << std::endl;
    std::cout << "  number of particles " << N        << std::endl;
    std::cout << "  number of MC steps  " << nMC      << std::endl;
    std::cout << "  temperature         " << temp     << std::endl;
    std::cout << "  step                " << step     << std::endl;
    std::cout << "  max_step            " << max_step << std::endl;
    std::cout << "  min_step            " << min_step << std::endl;
    std::cout << "  factor              " << factor   << std::endl;
    std::cout << "  number of tests     " << ntest    << std::endl;
    std::cout << "  yukawa constant     " << yukawa   << std::endl;
    std::cout << "  initial coordinates " << ini_coo  << std::endl;
    std::cout << "  verbose             " << verbose  << std::endl;
    std::cout << std::endl;

    std::cout << "=GROUND=STATE===============================";
    std::cout << std::endl << std::endl;
    std::cout << "  test                " << std::endl;
    std::cout << "    " << lowest_test    << std::endl << std::endl;
    std::cout << "  energy/N            " << std::endl;
    std::cout << "    " << lowest_energy  << std::endl << std::endl;
    std::cout << "  coordinates         " << std::endl;
    std::cout << "    " << lowest_coo     << std::endl << std::endl;

    std::cout << "=ENERGIES/N=================================";
    std::cout << std::endl << std::endl;
    for ( auto e : energies ) { std::cout << "  " << e << std::endl;}
    std::cout << std::endl;

		std::cout << "=STATISTICS=================================";
    std::cout << std::endl << std::endl;
		std::cout << "  Metropolis succeses " << std::endl;
		std::cout << "    " << ps/((double)N*ntest*nMC) << std::endl << std::endl;
		std::cout << "  Metropolis failures " << std::endl;
		std::cout << "    " << pf/((double)N*ntest*nMC) << std::endl << std::endl;
    std::cout << std::endl;


    return 0;
}



