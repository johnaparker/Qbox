#ifndef GUARD_field2_h
#define GUARD_field2_h
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include "matrix.h"
#include "sources/source.h"
#include "objects/object.h"
#include "objects/medium.h"
#include "monitors/monitor.h"
#include "pml.h"
#include "tfsf.h"
#include "timer.h"
#include "h5cpp.h"
#include "fieldIO.h"




namespace qbox {

    enum class fields;
    class Field2D;
    class source;
    class tfsf;
    class fieldIO;

    //grid class that controls length, resolution, and boundary of the grid
    class grid_properties {
    public:
        grid_properties(int Lx, int Ly, int res, int pml_thickness);
        grid_properties() = default;

        //functions to set TFSF
        void set_tfsf(std::vector<int> p1_val, std::vector<int> p2_val);  //direct corner specification
        void set_tfsf(int xbuff, int ybuff);   //xbuff is the length between simulation edge and start of TFSF
        void set_tfsf(int buff);    //xbuff = ybuff = buff

        //*** These should perhaps have <double> for real space, and a double res variable with it
        //conversions from grid to real, vice versa 
        std::vector<int> convertToGrid(std::vector<int> p);     //take real p into grid 
        std::vector<int> convertToReal(std::vector<int> pi);    //take grid pi into real

    public:
        bool totalFieldScatteredField;      //True if TFSF is in use
        int Lx, Ly;          //Actual length in x,y direction
        int Nx, Ny;          //Number of grid cells in x,y direction
        double dx;           //Actual length between grid points; dx = 1/res, Nx*dx = Lx
        int res;             //grid resolution (points per 1 Lx unit); Nx = Lx*res
        int pml_thickness;   //The thickness of the pml in # of grid cells
        std::vector<int> p1, p2;            //Vector positions of TFSF corners 
        //*** This should be a separate (boundary) class, and p1,p2 should volumes
    };



    //Field object that does all of the work
    class Field2D {
        friend class fieldIO;
    public:
        Field2D(grid_properties grid, std::string filename = "");
        void display_info();   //print basic info about system
        void update();         //update the fields for a single time step; also time steps sources/monitors
        
        //Add objects, sources, and monitors. These are all polymorphic classes
        void add_object(object &new_object);
        void add_source(source &new_source);
        void add_monitor(monitor &new_monitor);

        //write to HDF5 with filename
        void write_field(const fields);  //write nodename (Ex,etc.) to filename
        //*** This should take in a monitor??
        void write_monitor(std::string name, double *data, int N, bool extendable = false);   //write a monitor
        void writeE();    //write the E fields
        void writeH();    //write the H fields

    public:
        //Grid properties (some reduntant)
        grid_properties grid;  //grid object
        int Nx,Ny;             //# of cells
        double dx,Lx,Ly;       //Physical lengths
        int tStep;             //Current time step (starts at 0)

        double mu;             //Permeability constant everywhere

        //2D matrices defined everwhere. These make up the dominant memory usage
        matrix<double> Ez,Dz,Hx,Hy,Iz,ca,cb;  //Fields + auxillary fields
        matrix<object*> obj;    //Pointer-to-Object matrix

        //Objects, sources, and monitors. These are all polymorphic classes
        std::vector<object*> obj_list;
        std::vector<source*> source_list;
        std::vector<monitor*> monitor_list;

        std::unique_ptr<medium> background;

        //Physical timestep, time
        double dt,t;

        //PML + TFSF
        std::unique_ptr<pml> BC;
        std::unique_ptr<tfsf> total;   //This is nullptr if not in use

        //*** Should be different class to manage IO
        //map of all HDF5 output files
        //std::map<std::string, h5out> outFiles;
        std::unique_ptr<fieldIO> output;

        
        //*** Enum here? 
        //map of field names to matrix pointer data
        std::map<std::string, matrix<double>* > field_components;
        timers clocks;
    };   
}

#endif
