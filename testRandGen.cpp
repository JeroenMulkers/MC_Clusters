/*
    Project: Monte Carlo Project
    Filename: testRandGen.cpp
    Date: 21-03-2013
    Author: Jeroen Mulkers
    Mail: jeroen.mulkers@student.ua.ac.be

    Purpose: This program tests the quality of the 'Ran1' random generator
*/

#include <iostream>
#include <vector>
#include "Ran1.h"

int findPeriod(std::vector<double>);

int main (int argc, char* argv[]) {

    Ran1 RG(5);
    int N = 100;

    std::vector<double> list;

    for(int i=0; i<N; i++){
        double number = RG.getNumber();
        std::cout << number << std::endl;
        list.push_back(number);
    }

    std::cout << findPeriod(list) << std::endl;

    return 0;
}

int findPeriod(std::vector<double> vec){
// This function finds the minimal period of a vector

    bool found = false;
    int period;

    for(period=1; period<vec.size(); period++){

        for(int i=0; i<vec.size()-period; i++){
            if(vec[i] == vec[i+period]){
                found = true;
            } else {
                found = false;
                break;
            }
        }

        if (found == true) {
            std::cout << "found" << std::endl;
            break;
        }
    }

    return period;
}
