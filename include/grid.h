#ifndef GUARD_grid_h
#define GUARD_grid_h

#include "vec.h"

namespace qbox {

    //grid class that controls length, resolution, and boundary of the grid
    class grid_properties {
    public:
        grid_properties(int Lx, int Ly, int res, int pml_thickness);
        grid_properties() = default;

        //functions to set TFSF
        void set_tfsf(ivec p1_val, ivec p2_val);  //direct corner specification
        void set_tfsf(int xbuff, int ybuff);   //xbuff is the length between simulation edge and start of TFSF
        void set_tfsf(int buff);    //xbuff = ybuff = buff

        //*** These should perhaps have <double> for real space, and a double res variable with it
        //conversions from grid to real, vice versa 
        ivec convertToGrid(const ivec &p);     //take real p into grid 
        ivec convertToReal(const ivec &pi);    //take grid pi into real

    public:
        bool totalFieldScatteredField;      //True if TFSF is in use
        int Lx, Ly;          //Actual length in x,y direction
        int Nx, Ny;          //Number of grid cells in x,y direction
        double dx;           //Actual length between grid points; dx = 1/res, Nx*dx = Lx
        int res;             //grid resolution (points per 1 Lx unit); Nx = Lx*res
        int pml_thickness;   //The thickness of the pml in # of grid cells
        ivec p1, p2;            //Vector positions of TFSF corners 
        //*** This should be a separate (boundary) class, and p1,p2 should volumes
    };

}

#endif
