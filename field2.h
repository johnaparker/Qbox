#ifndef GUARD_field2_h
#define GUARD_field2_h

#include <vector>
#include <fstream>
#include "matrix.h"
#include "field1D/field.h"

struct grid_properties {
    int Nx, Ny;
    double dx;
    int pml_thickness;
    
    bool totalFieldScatteredField;
    std::vector<int> p1, p2;

    grid_properties(int Nx, int Ny, double dx, int pml_thickness):
        Nx(Nx), Ny(Ny), dx(dx), pml_thickness(pml_thickness) {
        totalFieldScatteredField = false;       
    }

    void set_tfsf(std::vector<int> p1, std::vector<int> p2){
        p1 = p1;
        p2 = p2;
        totalFieldScatteredField = true;
    }
};

class tfsf {
public:
   Field1D *inc;
   int ia,ib,ja,jb;

public:
    tfsf(grid_properties grid, double dt);
    tfsf() = default;
    tfsf(const tfsf&) = default;
    tfsf& operator=(const tfsf&) = default;
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
    tfsf *total;

    std::ofstream outE,outH;
public:
    Field2D(grid_properties grid, double dt);
    void write();
    void display_info(double tf);
    void pulse(double f);
    void update();
    void run(double time);    

};   

#endif
