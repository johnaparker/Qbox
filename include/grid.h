#ifndef GUARD_grid_h
#define GUARD_grid_h

#include "vec.h"

namespace qbox {

    //grid class that controls length, resolution, and boundary of the grid
    class grid_properties {
    public:
        grid_properties(double _Lx, double _Ly, double res, int pml_thickness);
        grid_properties() = default;

        //functions to set TFSF
        void set_tfsf(vec p1_val, vec p2_val);  //direct corner specification
        void set_tfsf(double xbuff, double ybuff);   //xbuff is the length between simulation edge and start of TFSF
        void set_tfsf(double buff);    //xbuff = ybuff = buff

        //conversions from grid to real, vice versa 
        ivec to_grid(const vec &p);     //take real p into grid 
        vec to_real(const ivec &pi);    //take grid pi into real

        isurface to_grid(const surface &surf);     //take real p into grid 
        surface to_real(const isurface &surf);    //take grid pi into real

        ivolume to_grid(const volume &vol);     //take real p into grid 
        volume to_real(const ivolume &vol);    //take grid pi into real

    public:
        bool totalFieldScatteredField;      //True if TFSF is in use
        double Lx, Ly;          //Actual length in x,y direction
        int Nx, Ny;          //Number of grid cells in x,y direction
        double dx;           //Actual length between grid points; dx = 1/res, Nx*dx = Lx
        double res;             //grid resolution (points per 1 Lx unit); Nx = Lx*res
        int pml_thickness;   //The thickness of the pml in # of grid cells
        ivec p1, p2;            //Vector positions of TFSF corners 
        //*** This should be a separate (boundary) class, and p1,p2 should volumes
    };

}

#endif
