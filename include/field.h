#ifndef GUARD_field_h
#define GUARD_field_h

#include "vec.h"
#include "sources/time_profile.h"

namespace qbox {

    struct pmlBoundary{
        Eigen::Array2d left; 
        Eigen::Array2d right; 
    };

    class Field1D {
    public:
        Field1D(int Nx, double dl, double dt);
        void pulse(const time_profile &tp);
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
