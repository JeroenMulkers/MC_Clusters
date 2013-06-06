const double PI = 3.14159265359;


struct Particle{
    double x;
    double y;
    double step;
};


typedef std::vector< Particle > Set_particles;


void initializeSet(Set_particles & set, Ran1 & RG, double step, std::string ini_coo){

    // Check if the number of particles in file is right
    if(ini_coo != "random"){
        int numberOfLines = 0;
        std::ifstream input_file(ini_coo);
        std::string line;
        while (std::getline(input_file, line)) {
            if (line[0] != '#' ){
                numberOfLines++;
                            }
        }
        if (numberOfLines != set.size()){
            std::cout << "error: file " << ini_coo << " doesn't exist" << std::endl;
            std::cout << "  the set will be initialized randomly" << std::endl;
            ini_coo = "random";
        }
    }


    // Initialize randomly
    if(ini_coo == "random"){

        // loop over every particle in the set
        int i = 0;
        for( auto & particle : set){

            double a = sqrt(0.25*(++i)*RG.getNumber());
            double rndm = RG.getNumber();

            particle.x = a*cos(2*PI*rndm);
            particle.y = a*sin(2*PI*rndm);
            particle.step = step;


        }

    // Initiliaze using data file
    } else {

        std::ifstream input_file(ini_coo);
        std::string line;
        int i=0;
        while (std::getline(input_file, line)) {
            if (line[0] != '#' )
            {
                double x,y,r;
                sscanf(line.c_str(),"%lf%lf%lf",&x,&y,&r);
                set[i].x = x;
                set[i].y = y;
                set[i].step = step;
                i++;
            }
        }
        input_file.close();

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

void PrintCoordinates(Set_particles set, double energy, int test, int step, char mode){

    int N = set.size();
    char filename[25];

    switch (mode){
        case 's':
            sprintf(filename, "test_%d_step_%d.coo", test, step);
            break;
        case 'i':
            sprintf(filename, "test_%d_initial.coo",test);
            break;
        case 'f':
            sprintf(filename, "test_%d_final.coo",test);
            break;
    }

    std::ofstream output(filename);
    output << "# Test " << test << std::endl;
    output << "# Step " << step << std::endl;
    output << "# Number of particles " << N << std::endl;
    output << "# Energy per particle " << energy/N << std::endl;
    output << "#==============================" << std::endl;
    output << "# x\ty\tr\talpha" << std::endl;
    output << std::setiosflags(std::ios::left);
    for( auto p : set ){
        output << std::setw(16) << p.x << std::setw(16) << p.y;
        output << std::setw(16) << pow( pow(p.x,2) + pow(p.y,2) , 0.5);
        output << std::setw(16) << atan2(p.y,p.x) << std::endl;
    }
    output.close();
}

