const double PI = 3.14159265359;

struct Particle{
    double x;
    double y;
    double step;
};

typedef std::vector< Particle > Set_particles;

void initializeSet(Set_particles & set, Ran1 & RG, double step){
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

double Energy(Set_particles set, double yukawa){
    //Calculates the energy of a given configuration

    double energy = 0;

    for(auto it1 = set.begin(); it1 != set.end(); it1++){

        Particle p1 = *it1;
        energy += pow(p1.x,2);
        energy += pow(p1.y,2);

        for(auto it2 = it1+1; it2 != set.end(); it2++){
            Particle p2 = *it2;
            double r = pow( pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2) , 0.5);
            energy += exp(-yukawa*r)/r;
        }
    }

    return energy;
}

void PrintCoordinates(Set_particles set, std::ostream & output){
    output << std::setiosflags(std::ios::left);
    for( auto p : set ){
        output << std::setw(16) << p.x << std::setw(16) << p.y;
        output << std::setw(16) << pow( pow(p.x,2) + pow(p.y,2) , 0.5) << std::endl;
    }
}
