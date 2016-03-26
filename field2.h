#ifndef GUARD_field2_h
#define GUARD_field2_h
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "field.h"
#include "matrix.h"
#include "object.h"
#include "source.h"
#include "h5out.h"
#include "monitor.h"

class Field2D;
class source;
class monitor;

//grid class that controls length, resolution, and boundary of the grid
class grid_properties {
public:
    grid_properties(int Lx, int Ly, int res, int pml_thickness);

    //functions to set TFSF
    void set_tfsf(std::vector<int> p1_val, std::vector<int> p2_val);  //direct corner specification
    void set_tfsf(int xbuff, int ybuff);   //xbuff is the length between simulation edge and start of TFSF
    void set_tfsf(int buff);    //xbuff = ybuff = buff

    //*** These should perhaps have <double> for real space, and a double res variable with it
    //conversions from grid to real, vice versa 
    std::vector<int> convertToGrid(std::vector<int> p);     //take real p into grid 
    std::vector<int> convertToReal(std::vector<int> pi);    //take grid pi into real

public:
    int Nx, Ny;          //Number of grid cells in x,y direction
    int Lx, Ly;          //Actual length in x,y direction
    int res;             //grid resolution (points per 1 Lx unit); Nx = Lx*res
    double dx;           //Actual length between grid points; dx = 1/res, Nx*dx = Lx
    int pml_thickness;   //The thickness of the pml in # of grid cells
    
    //*** This should be a separate (boundary) class, and p1,p2 should volumes
    bool totalFieldScatteredField;      //True if TFSF is in use
    std::vector<int> p1, p2;            //Vector positions of TFSF corners 
};


//TFSF object created by the grid_properties class
class tfsf {
public:
    //*** This class needs rule of 3 (or Zero)
    tfsf(grid_properties grid, double dt);
    tfsf() = default;
    tfsf(const tfsf&) = default;
    tfsf& operator=(const tfsf&) = default;

    void pulse();               //update the source to 1D sim
    void updateD(Field2D* f);   //update the D field in 1D sim
    void updateH(Field2D* f);   //update the H field in 1D sim

public:
   //*** Smart poiter here, and Field1D needs proper destructor
   Field1D *inc;       //A 1D field simulation to produce perfect plane wave
   int ia,ib,ja,jb;    //position  of corners
};


//PML boundary object
class pml {
public:
    //*** This class needs rule of 3 (or Zero)
    pml(grid_properties grid);
    pml() = default;
    pml(const pml&) = default;
    pml& operator=(const pml&) = default;

public:
    //*** Smart Pointers
    //PML array and matrix properties
    double *fi1,*fi2,*fi3,*fj1,*fj2,*fj3;
    double *gi2,*gi3,*gj2,*gj3;
    matrix<double> Ihx, Ihy;

    //PML dimensions
    int Nx, Ny;      //Nx, Ny of boundary_parameters
    int thickness;   //# of cells for thickness
};


//Field object that does all of the work
class Field2D {
public:
    Field2D(grid_properties grid);
    void display_info();   //print basic info about system
    void update();         //update the fields for a single time step; also time steps sources/monitors
    
    //Add objects, sources, and monitors. These are all polymorphic classes
    void add_object(object &new_object);
    void add_source(source &new_source);
    void add_monitor(monitor &new_monitor);

    //write to HDF5 with filename
    void write(std::string filename, std::string nodename);  //write nodename (Ex,etc.) to filename
    //*** This should take in a monitor??
    void write_monitor(std::string filename, std::string nodename, double *data, int N, bool extendable = false);   //write a monitor
    void writeE(std::string filename);    //write the E fields
    void writeH(std::string filename);    //write the H fields

public:
    //Grid properties (some reduntant)
    double mu;             //Permeability constant everywhere
    double dx,Lx,Ly;       //Physical lengths
    int Nx,Ny;             //# of cells
    int tStep;             //Current time step (starts at 0)
    grid_properties grid;  //grid object

    //2D matrices defined everwhere. These make up the dominant memory usage
    matrix<double> Ez,Dz,Hx,Hy,Iz,ca,cb;  //Fields + auxillary fields
    matrix<object*> obj;    //Pointer-to-Object matrix

    //Objects, sources, and monitors. These are all polymorphic classes
    std::vector<object*> obj_list;
    std::vector<source*> source_list;
    std::vector<monitor*> monitor_list;

    //Physical timestep, time
    double dt,t;

    //PML + TFSF
    pml BC;
    tfsf *total;   //This is nullptr if not in use

    //*** Should be different class to manage IO
    //map of all HDF5 output files
    std::map<std::string, h5out> outFiles;
    
    //*** Enum here? 
    //map of field names to matrix pointer data
    std::map<std::string, matrix<double>* > field_components;
};   

#endif
