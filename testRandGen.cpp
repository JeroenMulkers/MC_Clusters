/*
    Project: Monte Carlo Project
    Filename: testRandGen.cpp
    Date: 21-03-2013
    Author: Jeroen Mulkers
    Mail: jeroen.mulkers@student.ua.ac.be

    Purpose: This program tests the quality of the 'Ran1' random generator
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "Ran1.h"
#include <string>

void picture2D(const int N, const int seed, const std::string filename);
void testPeriod(const int N, const int seed);
int findPeriod(std::vector<double>);

int main (int argc, char* argv[]) {

    testPeriod(1000000,5);
    picture2D(1000,5,"random2D.dat");

    return 0;
}

void testPeriod(const int N, const int seed){

    Ran1 RG(seed);

    std::vector<double> list;
    for(int i=0; i<N; i++)
        list.push_back(RG.getNumber());

    std::cout << findPeriod(list) << std::endl;
}

void picture2D(const int N, const int seed, const std::string filename){

    Ran1 RG(seed);

    std::ofstream output(filename);

    for(int i=0; i<N; i++){
        double x = RG.getNumber();
        double y = RG.getNumber();
        output << x << "\t" << y << std::endl;
    }

    output.close();

}

int findPeriod(std::vector<double> vec){
// This function finds the minimal period of a vector

    bool found = false;
    int period;

    for(period=1; period<vec.size(); period++){

        for(int i=0; i<vec.size()-period; i++){
            if(vec[i] == vec[i+period])
                found = true;
            else
                found = false; break;
        }

        if (found == true) {
            std::cout << "found" << std::endl;
            break;
        }
    }

    return period;
}


