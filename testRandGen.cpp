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
#include <math.h>

void testPeriod(const int N, const int seed);
void picture2D(const int N, const int seed, const std::string filename);
void chi2test();
void autocorrelation();

//helper functions
int findPeriod(std::vector<double>);
double chi2_calc(const int nBins, const std::vector<double> xvec);

int main (int argc, char* argv[]) {

    // testPeriod(1000000,5);
    // picture2D(10000,5,"random2D.dat");
    chi2test();
    autocorrelation();

    return 0;
}

void testPeriod(const int N, const int seed){

    Ran1 RG(seed);

    std::vector<double> list;
    for(int i=0; i<N; i++)
        list.push_back(RG.getNumber());

    std::cout << "The period is greater than or equal to " << findPeriod(list) << std::endl;
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

void chi2test(){
    int N = 10000;
    int nBins = 10;
    Ran1 RG(5);

    std::vector<double> list;

    for(int i=0; i<N; i++)
        list.push_back(RG.getNumber());

    double chi2 = chi2_calc(nBins, list);
    std::cout << "chi2 = " << chi2 << std::endl;
}

double chi2_calc(const int nBins, const std::vector<double> xvec){

    // Expectation value
    double E = xvec.size() / (double) nBins;

    double chi2 = 0;

    std::vector<int> bins;
    bins.assign(nBins,0);

    for( auto x : xvec)
        bins[floor(x*nBins)]++;

    for( auto bin : bins )
        chi2 += pow(bin-E,2)/E;

    return chi2;
}

void autocorrelation(){
    int N = 1000;
    int seed = 5;
    int k = 100;

    Ran1 RG(seed);
    std::vector<double> list;

    for(int i=0; i<N; i++)
        list.push_back(RG.getNumber());

    double Xi = 0.;
    double XiXi = 0.;
    double XkiXi = 0.;

    for(int i=0; i<N; i++){
        Xi    += list[i];
        XiXi  += list[i]*list[i];
        if(k+i < list.size())
            XkiXi += list[i]*list[k+i];
    }

    Xi    /= N;
    XiXi  /= N;
    XkiXi /= N;

    double C = ( XkiXi - Xi*Xi ) / ( XiXi - Xi*Xi );
    std::cout << C << std::endl;
}
