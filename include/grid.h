#ifndef GUARD_grid_h
#define GUARD_grid_h

#include "vec.h"
#include "h5cpp.h"
#include "qbox/sources/time_profile.h"

namespace qbox {

    //grid class that controls length, resolution, and boundary of the grid
    class grid_properties {
    public:
        grid_properties() = default;
        grid_properties(double _Lx, double _Ly, double res, int pml_thickness);

        //conversions from grid to real, vice versa 
        ivec to_grid(const vec &p) const;     //take real p into grid 
        vec to_real(const ivec &pi) const;    //take grid pi into real

        isurface to_grid(const surface &surf) const;     //take real p into grid 
        surface to_real(const isurface &surf) const;    //take grid pi into real

        ivolume to_grid(const volume &vol) const;     //take real p into grid 
        volume to_real(const ivolume &vol) const;    //take grid pi into real

        void write(const h5cpp::h5group &group);

    public:
        bool totalFieldScatteredField;      //True if TFSF is in use
        double Lx, Ly;          //Actual length in x,y direction
        int Nx, Ny;          //Number of grid cells in x,y direction
        double dx;           //Actual length between grid points; dx = 1/res, Nx*dx = Lx
        double dt;           //time step
        double res;             //grid resolution (points per 1 Lx unit); Nx = Lx*res
        int pml_thickness;   //The thickness of the pml in # of grid cells
        //*** This should be a separate (boundary) class, and p1,p2 should volumes
    };

}

#endif
