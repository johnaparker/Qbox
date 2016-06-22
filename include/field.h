#ifndef GUARD_field_h
#define GUARD_field_h

#include <vector>
#include <fstream>

namespace qbox {
    const double c0 = 2.99792458e8;
    const double epsilon = 8.854e-12;

    struct pmlBoundary{
        double *left = new double[2];
        double *right = new double[2];
    };

    class Field1D {
    public:
        Field1D(int Nx, double dl, double dt);
        void write();
        void display_info();
        void pulse(double f);
        void update();
        void run(double time);    

    public:
        int Nx,tStep;
        double dx,L;
        double dt,t;

        double mu;

        double *Ez,*Dz,*Hx,*Iz,*ca,*cb;
        pmlBoundary pml;

        std::ofstream outE,outH;

    };   
}

#endif
