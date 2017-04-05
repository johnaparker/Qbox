#ifndef GUARD_field_h
#define GUARD_field_h

#include "vec.h"

namespace qbox {

    struct pmlBoundary{
        Eigen::Array2d left; 
        Eigen::Array2d right; 
    };

    class Field1D {
    public:
        Field1D(int Nx, double dl, double dt);
        void pulse(double f);
        void update();

    public:
        int Nx,tStep;
        double dx,L;
        double dt,t;

        Array Ez,Dz,Hx,Iz,ca,cb;
        pmlBoundary pml;

    };   
}

#endif
