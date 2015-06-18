#ifndef GUARD_field_h
#define GUARD_field_h

#include <vector>
#include <fstream>

const double c0 = 3e8;

class Field1D {
public:
    double mu;
    double dx,L;
    int Nx,tStep;

    double *Ex,*Hy,*epsinv;
    double t;

    std::ofstream outE,outH;
public:
    Field1D(double length, double dl);
    void write();
    void display_info();
    void pulse(double f);
    void update(double dt);
    void run(double time,double dt);    

};   

#endif
