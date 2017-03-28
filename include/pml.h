#ifndef GUARD_pml_h
#define GUARD_pml_h

#include <memory>
#include "matrix.h"
#include "vec.h"

namespace qbox {
    class grid_properties;

    //PML boundary object
    class pml {
    public:
        pml(grid_properties grid);
        pml() = default;

    public:
        //PML array and matrix properties
        Array fi1,fi2,fi3,fj1,fj2,fj3;
        Array gi2,gi3,gj2,gj3;
        matrix<double,2> Ihx, Ihy;

    private:
        //PML dimensions
        int Nx, Ny;      //Nx, Ny of boundary_parameters
        int thickness;   //# of cells for thickness
    };
}

#endif
