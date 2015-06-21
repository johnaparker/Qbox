#ifndef GUARD_field_h
#define GUARD_field_h

#include <vector>
#include <fstream>

const double c0 = 3e8;
const double epsilon = 8.854e-12;

class Field1D {
public:
    double mu;
    double dx,L;
    int Nx,tStep;

    double *Ex,*Dx,*Hy,*Ix,*ca,*cb;
    double dt,t;

    std::ofstream outE,outH;
public:
    Field1D(double length, double dl, double dt);
    void write();
    void display_info();
    void pulse(double f);
    void update();
    void run(double time);    

};   

#endif
