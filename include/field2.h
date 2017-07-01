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
#include "time_monitors/time_monitor.h"
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

    template<class T> struct always_false : std::false_type {};

    extern const std::map<fields,std::string> field_names;

    //Field object that does all of the work
    class Field2D {
    public:
        Field2D(grid_properties grid, std::string filename = "");
        void display_info();   //print basic info about system
        void update();         //update the fields for a single time step; also time steps sources/monitors
        void update_material_grid();
        
        //Add objects, sources, and monitors. These are all polymorphic classes
        template<class T, class... Args>
        T& add(Args&&... args);

        void remove_monitors();
        void clear_fields();
        void clear_materials();          ///< Set all grid points to have material of the background
        void reset_tfsf();

        const tfsf& set_tfsf(const volume& vol, const time_profile& tp);
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
        void insert_object(object &new_object);
        void insert_object_helper(object &new_object, const material& mat);
        void update_object_material_grid(const object &obj);    ///< update material grid points for a given object
        void update_object_polarization_grid(const object &obj);    ///< update polarization grid points for a given object

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
        std::vector<std::unique_ptr<object>>          obj_list;
        std::vector<std::unique_ptr<dynamic_object>>  dynamic_objects;
        std::vector<std::unique_ptr<source>>          source_list;
        std::vector<std::unique_ptr<monitor>>         monitor_list;
        std::vector<std::unique_ptr<time_monitor>>    time_monitor_list;

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

    template<class T, class... Args>
    T& Field2D::add(Args&&... args) {

        if constexpr (std::is_base_of<object,T>::value) {
            std::unique_ptr<object> new_object = std::make_unique<T>(std::forward<Args>(args)...);
            insert_object(*new_object);
            new_object->set_owner(this);
            obj_list.push_back(std::move(new_object));
            return static_cast<T&>(*obj_list.back());
        }

        else if constexpr (std::is_base_of<monitor,T>::value) {
            std::unique_ptr<monitor> new_monitor = std::make_unique<T>(std::forward<Args>(args)...);
            new_monitor->set_F(this);
            monitor_list.push_back(std::move(new_monitor));
            return static_cast<T&>(*monitor_list.back());
        }

        else if constexpr (std::is_base_of<time_monitor,T>::value) {
            std::unique_ptr<time_monitor> new_monitor = std::make_unique<T>(std::forward<Args>(args)...);
            new_monitor->set_F(this);
            time_monitor_list.push_back(std::move(new_monitor));
            return static_cast<T&>(*time_monitor_list.back());
        }

        else if constexpr (std::is_base_of<source,T>::value) {
            std::unique_ptr<source> new_source = std::make_unique<T>(std::forward<Args>(args)...);
            new_source->set_F(this);
            source_list.push_back(std::move(new_source));
            return static_cast<T&>(*source_list.back());
        }

        else {
            static_assert(always_false<T>::value, "type 'T' is not allowed to be added");
        }
    }
    

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
