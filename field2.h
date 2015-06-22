#ifndef GUARD_field2_h
#define GUARD_field2_h

#include <vector>
#include <fstream>

const double c0 = 3e8;
const double epsilon = 8.854e-12;

class Field2D {
public:
    double mu;
    double dx,Lx,Ly;
    int Nx,Ny,tStep;

    double *Ez,*Dz,*Hx,*Hy,*Iz,*ca,*cb;
    double dt,t;

    std::ofstream outE,outH;
public:
    Field2D(double *dim, double dl, double dt);
    void write();
    void display_info(double tf);
    void pulse(double f);
    void update();
    void run(double time);    

};   

#endif
