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
#include <optional>
#include "sources/source.h"
#include "object.h"
#include "dynamic_object.h"
#include "geometry/geometry.h"
#include "monitors/monitor.h"
#include "pml.h"
#include "tfsf.h"
#include "timer.h"
#include "vec.h"
#include "h5cpp.h"
#include "grid.h"

#include "materials/debye_polarization.h"
#include "materials/drude_polarization.h"
#include "materials/lorentz_polarization.h"

namespace qbox {

    class source;
    class tfsf;

    //Field object that does all of the work
    class Field2D {
    public:
        Field2D(grid_properties grid, std::string filename = "");
        void display_info();   //print basic info about system
        void update();         //update the fields for a single time step; also time steps sources/monitors
        void update_material_grid();
        
        //Add objects, sources, and monitors. These are all polymorphic classes
        void add_object(object &new_object);

        void add_source(source &new_source);
        void add_monitor(monitor &new_monitor);

        void clear_monitors();
        void clear_fields();
        void clear_materials();          ///< Set all grid points to have material of the background
        void reset_tfsf();

        void set_tfsf(const volume& vol, const time_profile& tp);
        void set_tfsf_freq(const freq_data &freq);
        void write_tfsf();

        //write to HDF5 with filename
        void write_field(const fields);  //write nodename (Ex,etc.) to filename
        //*** This should take in a monitor??
        void write_monitor(monitor& mon);   //write a monitor
        void writeE();    //write the E fields
        void writeH();    //write the H fields

        double interpolate(fields C, const vec &p);
        double to_grid(fields C, const ivec &pi);
        double to_xgrid(fields C, const ivec &pi);
        double to_ygrid(fields C, const ivec &pi);

        tensor& get_field_ref(fields F);

    private:
        void create_fields_dataset(fields field);
        void add_object(object &new_object, const material& mat);

    public:
        //Grid properties (some reduntant)
        grid_properties grid;  //grid object
        int Nx,Ny;             //# of cells
        double dx,Lx,Ly;       //Physical lengths
        int tStep;             //Current time step (starts at 0)

        double mu;             //Permeability constant everywhere

        //2D matrices defined everwhere. These make up the dominant memory usage
        tensor Ez,Hx,Hy,Ca,Cb,Da,Db;  //Fields + auxillary fields

        //Objects, sources, and monitors. These are all polymorphic classes
        std::vector<object*> obj_list;
        std::vector<dynamic_object*> dynamic_objects;
        std::vector<source*> source_list;
        std::vector<monitor*> monitor_list;

        //list of materials added so far
        std::vector<std::string> materials_added;

        //background medium
        std::unique_ptr<material> background;

        //Physical timestep, time
        double dt,t;

        //PML + Auxillary fields
        std::unique_ptr<pml> BC;
        Array khdx, khdy, kedx, kedy;     //scaling factors for pml
        std::optional<tensor> prevE;
        std::optional<tensor> prev2E;

        //TFSF
        std::optional<tfsf> total;   //This is nullptr if not in use

        //Polarization auxillary fields, map from material_name -> polarization object
        std::map<std::string, debye_polarization>   P_debye;
        std::map<std::string, drude_polarization>   P_drude;
        std::map<std::string, lorentz_polarization> P_lorentz;

        //*** Should be different class to manage IO
        //map of all HDF5 output files
        //std::map<std::string, h5out> outFiles;
        std::unique_ptr<h5cpp::h5file> outFile;
        
        //*** Enum here? 
        //map of field names to tensor pointer data
        std::map<std::string, tensor* > field_components;
        timers clocks;
    };

    template <class POL, class MAT>
    void add_polarization(std::map<std::string,POL> &P_map, const MAT &mat,
            const object &new_object, const grid_properties &grid) {
        auto name = mat.get_name();
        auto iter = P_map.find(name);
        if (iter == P_map.end()) {
            auto new_P = POL(grid, mat);
            new_P.insert_object(new_object);
            P_map.insert(std::pair<std::string,POL>(name, new_P));
        }
        else {
            iter->second.insert_object(new_object);
        }
    }

}

#endif
