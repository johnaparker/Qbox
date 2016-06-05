#ifndef GUARD_field2_h
#define GUARD_field2_h
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "matrix.h"
#include "object.h"
#include "source.h"
#include "monitor.h"
#include "surface_monitor.h"
#include "box_monitor.h"
#include "pml.h"
#include "tfsf.h"

#include "h5out.h"
#include "field.h"


namespace apine {

    class Field2D;
    class source;
    class tfsf;

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
}

#endif
