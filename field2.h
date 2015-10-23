#ifndef GUARD_field2_h
#define GUARD_field2_h

#include <vector>
#include <fstream>
#include "field.h"
#include "matrix.h"
#include "object.h"
#include "source.h"
#include "h5out.h"

class Field2D;
class source;

class grid_properties {
public:
    int Nx, Ny;
    double dx;
    int pml_thickness;
    
    bool totalFieldScatteredField;
    std::vector<int> p1, p2;

public:
    grid_properties(int Nx, int Ny, double dx, int pml_thickness);
    void set_tfsf(std::vector<int> p1_val, std::vector<int> p2_val);
    void set_tfsf(int xbuff, int ybuff);
    void set_tfsf(int buff);
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

    void pulse();
    void updateD(Field2D* f);
    void updateH(Field2D* f);
};



class pml {
public:
    double *fi1,*fi2,*fi3,*fj1,*fj2,*fj3;
    double *gi2,*gi3,*gj2,*gj3;
    matrix<double> Ihx, Ihy;
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

    matrix<double> Ez,Dz,Hx,Hy,Iz,ca,cb;
    matrix<object*> obj;
    std::vector<object*> obj_list;
    std::vector<source*> source_list;
    double dt,t;

    pml BC;
    tfsf *total;

    h5out outFile;
public:
    Field2D(grid_properties grid, double dt);
    void write();
    void display_info(double tf);
    void pulse(double f);
    void update();
    void run(double time);    

    void add_object(object &new_object);
    void add_source(source &new_source);
};   

#endif
