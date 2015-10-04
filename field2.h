#ifndef GUARD_field2_h
#define GUARD_field2_h

#include <vector>
#include <fstream>
#include "matrix.h"

const double c0 = 3e8;
const double epsilon = 8.854e-12;

struct grid_properties {
    int Nx, Ny;
    double dx;
    int pml_thickness;
    
    bool totalFieldScatteredField;
    std::vector<int> p1, p2;

    grid_properties(int Nx, int Ny, double dx, int pml_thickness):
        Nx(Nx), Ny(Ny), dx(dx), pml_thickness(pml_thickness) {}
    tfsf(std::vector<int>, std::vector<int>)
};

class tfsf {
public:
   Field1D *inc;
   int ia,ib,ja,jb;

public:
    tfsf(grid_properties grid);
    tfsf() = default;
    tfsf(const pml&) = default;
    tfsf& operator=(const pml&) = default;
};

class pml {
public:
    double *fi1,*fi2,*fi3,*fj1,*fj2,*fj3;
    double *gi2,*gi3,*gj2,*gj3;
    matrix Ihx, Ihy;
    int Nx, Ny;
    int thickness;

public:
    pml(grid_properties grid);
    pml() = default;
    pml(const pml&) = default;
    pml& operator=(const pml&) = default;
};

class Field2D {
public:
    double mu;
    double dx,Lx,Ly;
    int Nx,Ny,tStep;

    matrix Ez,Dz,Hx,Hy,Iz,ca,cb;
    double dt,t;

    pml BC;

    std::ofstream outE,outH;
public:
    Field2D(grid_properties grid, double dx, double dt);
    void write();
    void display_info(double tf);
    void pulse(double f);
    void update();
    void run(double time);    

};   

#endif
