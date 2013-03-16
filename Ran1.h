#ifndef _RAN1_H_
#define _RAN1_H_

class Ran1 {

public:
    Ran1 (int seed);
    virtual ~Ran1 ();
    void setSeed(int seed);
    double getNumber();

private:
    double R[97];
    int ix1;
    int ix2;
    int ix3;

};

#endif
