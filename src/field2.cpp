#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <string>
#include "field2.h"
#include "field.h"
#include "matrix.h"
#include "tfsf.h"
#include "termcolor.h"
#include "timer.h"

#include "materials/simple_material.h"
#include "geometry/cylinder.h"

using namespace std;

namespace qbox {

    Field2D::Field2D(grid_properties grid, string filename): grid(grid), Nx(grid.Nx), Ny(grid.Ny), dx(grid.dx), Lx(grid.Lx), Ly(grid.Ly) {
        t = 1;
        tStep = 0;
        dt = dx/2.0;
        Ez = matrix<double,2>(Nx, Ny); 
        Hx = matrix<double,2>(Nx, Ny);
        Hy = matrix<double,2>(Nx, Ny);
        Dz = matrix<double,2>(Nx, Ny);
        Iz = matrix<double,2>(Nx, Ny);
        ca = matrix<double,2>(Nx, Ny);
        cb = matrix<double,2>(Nx, Ny);

        obj  = matrix<object*,2>(Nx, Ny); 

        background = make_unique<simple_material>(1);

        for (int i = 0; i != Nx; i++) {
            for (int j = 0; j != Ny; j++) {
                double eps = background->get_eps();      
                double conduc = background->get_conduc(); 
                ca(i,j) = 1/(eps + conduc*dt/epsilon);
                cb(i,j) = conduc*dt/epsilon;
                obj(i,j) = nullptr;
             }
        }

        BC = make_unique<pml>(grid); 

        if (grid.totalFieldScatteredField) 
            total = make_unique<tfsf>(grid, dt);
        else
            total = nullptr;
        field_components = {{"Ez", &Ez}, {"Hx", &Hx}, {"Hy", &Hy}};

        if (!filename.empty()) {
            outFile = make_unique<h5cpp::h5file>(filename, h5cpp::io::w);
            cout << "\n" << termcolor::bold << termcolor::green 
                 << "New simulation, creating HDF5 output file \'" << filename << "\'" 
                 << termcolor::reset << endl << endl;
        }


        display_info();
    }

    void Field2D::write_field(const fields field) {
        clocks.start(clock_name::hdf5);
        switch(field) {
            case fields::Ez: if (!outFile->object_exists("Fields/Ez")) {
                                     create_fields_dataset(field);
                             }
                             else {
                                 auto gFields = outFile->open_group("Fields");
                                 auto dset = gFields.open_dataset("Ez");
                                 dset.append(Ez.data());
                             }
                             break;
            case fields::Hx: if (!outFile->object_exists("Fields/Hx")) {
                                     create_fields_dataset(field);
                             }
                             else {
                                 auto gFields = outFile->open_group("Fields");
                                 auto dset = gFields.open_dataset("Hx");
                                 dset.append(Hx.data());
                             }
                             break;
            case fields::Hy: if (!outFile->object_exists("Fields/Hy")) {
                                     create_fields_dataset(field);
                             }
                             else {
                                 auto gFields = outFile->open_group("Fields");
                                 auto dset = gFields.open_dataset("Hy");
                                 dset.append(Hy.data());
                             }
                             break;
            default: throw std::invalid_argument("not a valid field component");
        }
        clocks.stop(clock_name::hdf5);
    }

    void Field2D::writeE() {
        write_field(fields::Ez);
    }

    void Field2D::writeH() {
        write_field(fields::Hx);
        write_field(fields::Hy);
    }

    void Field2D::write_monitor(monitor& mon) {
        clocks.start(clock_name::hdf5);
        //output->write_monitor(mon);
        clocks.stop(clock_name::hdf5);
    }

    void Field2D::display_info() {
        const int max_spacing = 5;
        cout << termcolor::bold << termcolor::underline << "Grid data" 
             << termcolor::reset << endl; 
        cout << "     " << left << setw(max_spacing) 
             << setfill('.') << "dx" << dx << " m" << endl;
        cout << "     " << left << setw(max_spacing) 
             << setfill('.') << "Lx" << dx*Nx << " m" << endl;
        cout << "     " << left << setw(max_spacing) 
             << setfill('.') << "Ly" << dx*Ny << " m" << endl;
        cout << "     " << left << setw(max_spacing) 
             << setfill('.') << "dt" << dt << " s" << endl << endl;
    }

    //inside each for loop: make a call to an external function that makes the necessary update: vacuum, material, pml
    void Field2D::update() {
        tStep += 1;
        t += dt;

        if (tStep%100 == 0) {
            cout << "\rOn time step " << tStep << flush;
        }
        
        clocks.start(clock_name::looping);
        for (int i=1; i<Nx-1; i++) {
            for (int j=1; j<Ny-1; j++) {
                Dz(i,j) = BC->gi3[i]*BC->gj3[j]*Dz(i,j) + 
                    BC->gi2[i]*BC->gj2[j]*0.5*(Hy(i,j)-Hy(i-1,j) - Hx(i,j)+Hx(i,j-1));
             }
        }

        if (total) 
            total->updateD(this);

        for (int i=1; i<Nx-1; i++) {
            for (int j=1; j<Ny-1; j++) {
                Ez(i,j)= ca(i,j)*(Dz(i,j) -Iz(i,j));
                Iz(i,j)+= cb(i,j)*Ez(i,j);
             }
        }

        for (const auto &s : source_list) {
            s->pulse();
        } 
        clocks.stop(clock_name::looping);

        clocks.start(clock_name::fourier);
        for (const auto &m : monitor_list) {
            m->update();
        }
        clocks.stop(clock_name::fourier);


        clocks.start(clock_name::looping);
        //this can possibly be moved to the previous if statement
        if (total) 
            total->pulse();

        for (int i=1; i<Nx-1; i++) {
            for (int j=1; j<Ny-1; j++) {
                double curl_e = Ez({i+1,j}) - Ez({i,j});
                BC->Ihy(i,j) += curl_e;
                Hy(i,j) = BC->fi3[i]*Hy(i,j) + BC->fi2[i]*0.5*curl_e + BC->fj1[j]*BC->Ihy(i,j);
                
                curl_e = Ez(i,j) - Ez(i,j+1);
                BC->Ihx(i,j) += curl_e;
                Hx(i,j) = BC->fj3[j]*Hx(i,j) + BC->fj2[j]*0.5*curl_e + BC->fi1[i]*BC->Ihx(i,j);
            }
        }

        if (total) 
            total->updateH(this);
        clocks.stop(clock_name::looping);
        
    }

    void Field2D::add_object(object &new_object) {
        obj_list.push_back(&new_object);
        new_object.set_owner(this);
        double eps = new_object.get_material()->get_eps();
        double conduc = new_object.get_material()->get_conduc();

        for (int i = 0; i != Nx; i++) {
            for (int j = 0; j != Ny; j++) {
                ivec pi = {i,j};
                vec p = grid.to_real(pi);

                if (new_object.inside(p)) {
                    obj(pi) = &new_object;
                    obj(pi) = &new_object;
                        
                    ca(pi) = 1/(eps + conduc*dt/epsilon);
                    cb(pi) = conduc*dt/epsilon;
                }
            }
        }
    }

    void Field2D::add_source(source &new_source) {
        new_source.set_F(this);
        source_list.push_back(&new_source);
    }

    void Field2D::add_monitor(monitor &new_monitor) {
        new_monitor.set_F(this);
        monitor_list.push_back(&new_monitor);
    } 

    double Field2D::interpolate(fields C, const vec &p) {
        vec ps = p/dx; 
        auto &F = get_field_ref(C);

        switch(C) {
            case fields::Ez: break;
            case fields::Hx: ps -= vec(0, dx/2.0); break;
            case fields::Hy: ps -= vec(dx/2.0, 0); break;
            default: throw std::invalid_argument("not a valid field component");
        }

        ivec c1(floor(ps[0]), floor(ps[1]));
        ivec c2(ceil(ps[0]), floor(ps[1]));
        ivec c3(ceil(ps[0]), ceil(ps[1]));
        ivec c4(floor(ps[0]), ceil(ps[1]));

        double x = ps[0] - c1[0];
        double y = ps[1] - c1[1];

        double a1 = (1 - x)*(1 - y);
        double a2 = x*(1 - y);
        double a3 = x*y;
        double a4 = (1 - x)*y;

        return a1*F(c1) + a2*F(c2) + a3*F(c3) + a4*F(c4);
    }

    double Field2D::to_grid(fields C, const ivec &pi) {
        switch(C) {
            case fields::Ez:  return Ez(pi); break; 
            case fields::Hx:  return (Hx(pi) + Hx(pi - ivec(0,1)))/2; break; 
            case fields::Hy:  return (Hy(pi) + Hy(pi - ivec(1,0)))/2; break; 
            default: throw std::invalid_argument("not a valid field component");
        }
    }

    double Field2D::to_xgrid(fields C, const ivec &pi) {
        switch(C) {
            case fields::Ez:  return (Ez(pi) + Ez(pi + ivec(1,0)))/2; break; 
            case fields::Hx:  return (Hx(pi) + Hx(pi - ivec(1,0)) + Hx(pi + ivec(0,1)) + Hx(pi + ivec(-1,1)))/4; break; 
            case fields::Hy:  return Hy(pi); break; 
            default: throw std::invalid_argument("not a valid field component");
        }
    }

    double Field2D::to_ygrid(fields C, const ivec &pi) {
        switch(C) {
            case fields::Ez:  return (Ez(pi) + Ez(pi + ivec(0,1)))/2; break; 
            case fields::Hx:  return Hx(pi); break; 
            case fields::Hy:  return (Hy(pi) + Hy(pi - ivec(1,0)) + Hy(pi + ivec(0,1)) + Hy(pi + ivec(-1,1)))/4; break; 
            default: throw std::invalid_argument("not a valid field component");
        }
    }

    matrix<double,2>& Field2D::get_field_ref(fields F) {
        switch(F) {
            case fields::Ez:  return Ez; break; 
            case fields::Hx:  return Hx; break; 
            case fields::Hy:  return Hy; break; 
            default: throw std::invalid_argument("not a valid field component");
        }
    }

    void Field2D::create_fields_dataset(fields field) {
        h5cpp::h5group gFields;
        h5cpp::h5dset dset;

        if (!outFile->object_exists("Fields")) {
            gFields = outFile->create_group("Fields");
            h5cpp::dspace ds_a(vector<hsize_t>{1});
            auto attr = gFields.create_attribute("dx", h5cpp::dtype::Double, ds_a);
            attr.write(&(dx));
            attr = gFields.create_attribute("dt", h5cpp::dtype::Double, ds_a);
            attr.write(&(dt));
        }
        else
            gFields = outFile->open_group("Fields");

        vector<hsize_t> dims = {hsize_t(Nx),hsize_t(Ny),1};
        vector<hsize_t> max_dims = {hsize_t(Nx),hsize_t(Ny),h5cpp::inf};
        vector<hsize_t> chunk_dims = dims;
        h5cpp::dspace ds(dims, max_dims, chunk_dims, false);
        switch(field) {
            case fields::Ez:  dset = gFields.create_dataset("Ez", 
                                     h5cpp::dtype::Double, ds); 
                              dset.write(Ez.data());
                              break;
            case fields::Hx:  dset = gFields.create_dataset("Hx", 
                                     h5cpp::dtype::Double, ds); 
                              dset.write(Hx.data());
                              break;
            case fields::Hy:  dset = gFields.create_dataset("Hy", 
                                     h5cpp::dtype::Double, ds); 
                              dset.write(Hy.data());
                              break;
            default: throw std::invalid_argument("not a valid field component");
        }
    }

}





