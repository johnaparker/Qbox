#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include "field2.h"
#include "tfsf.h"
#include "termcolor.h"
#include "timer.h"

#include "geometry/cylinder.h"

using namespace std;

namespace qbox {

    Field2D::Field2D(grid_properties grid, string filename): grid(grid), Nx(grid.Nx), Ny(grid.Ny), dx(grid.dx), Lx(grid.Lx), Ly(grid.Ly) {
        t = 0;
        tStep = 0;
        dt = grid.dt;
        Ez = tensor(Nx,Ny); Ez.setZero(); 
        Hx = tensor(Nx,Ny); Hx.setZero();
        Hy = tensor(Nx,Ny); Hy.setZero();
        
        Ca = tensor(Nx,Ny);
        Cb = tensor(Nx,Ny);
        Da = tensor(Nx,Ny);
        Db = tensor(Nx,Ny);

        background = make_unique<simple_material>(1);

        clear_materials();

        BC = make_unique<pml>(grid); 
        BC->set_scaling_factors(kedx, khdx, kedy, khdy);

        field_components = {{"Ez", &Ez}, {"Hx", &Hx}, {"Hy", &Hy}};

        if (!filename.empty()) {
            outFile = make_unique<h5cpp::h5file>(filename, h5cpp::io::w);
            cout << "\n" << termcolor::bold << termcolor::green 
                 << "New simulation, creating HDF5 output file \'" << filename << "\'" 
                 << termcolor::reset << endl << endl;
        }

        grid.write(outFile->create_group("grid"));
        display_info();
    }

    void Field2D::write_field(const fields field) {
        clocks.start(clock_name::hdf5);
        auto gFields = outFile->create_or_open_group("fields");
        switch(field) {
            case fields::Ez: if (!gFields.object_exists("Ez")) {
                                     create_fields_dataset(field);
                             }
                             else {
                                 auto dset = gFields.open_dataset("Ez");
                                 dset.append(Ez.data());
                             }
                             break;
            case fields::Hx: if (!gFields.object_exists("Hx")) {
                                     create_fields_dataset(field);
                             }
                             else {
                                 auto dset = gFields.open_dataset("Hx");
                                 dset.append(Hx.data());
                             }
                             break;
            case fields::Hy: if (!gFields.object_exists("fields/Hy")) {
                                     create_fields_dataset(field);
                             }
                             else {
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
        if (prev2E)
            prev2E = prevE;
        if (prevE)
            prevE = Ez;

        for (int i=1; i<Nx-1; i++) {
#pragma GCC ivdep
            for (int j=1; j<Ny-1; j++) {
                Ez(i,j) = Ca(i,j)*Ez(i,j) 
                    + Cb(i,j)*((Hy(i,j) - Hy(i-1,j))/kedx(i) + (Hx(i,j-1) - Hx(i,j))/kedy(j));
             }
        }
        
        BC->update_E(*this);

        for (auto &P : P_debye)
            P.second.update_J(*this);

        for (auto &P : P_drude)
            P.second.update_J(*this);

        for (auto &P : P_lorentz)
            P.second.update_J(*this);

        if (total) 
            total->updateD(this);

        //*** this is when electric sources are hit...magnetic sources should come after H is updated
        //*** also.... multiply all sources by the appropiate pre-factor
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
        //*** this can possibly be moved to the previous if statement
        if (total) 
            total->pulse();

        for (int i=1; i<Nx-1; i++) {
#pragma GCC ivdep
            for (int j=1; j<Ny-1; j++) {
                Hx(i,j) = Da(i,j)*Hx(i,j) - Db(i,j)*(Ez(i,j+1) - Ez(i,j))/khdy(j);
                Hy(i,j) = Da(i,j)*Hy(i,j) + Db(i,j)*(Ez(i+1,j) - Ez(i,j))/khdx(i);
            }
        }

        BC->update_H(*this);

        if (total) 
            total->updateH(this);

        clocks.stop(clock_name::looping);
        
    }

    void Field2D::update_material_grid() {
        clear_materials();
        
        for (auto obj_ptr : obj_list) {
            update_object_material_grid(*obj_ptr);
        }
    }

    void Field2D::add_object(object &new_object) {
        material_variant m = new_object.get_material();

        visit([this,&new_object](auto&& arg) {

            using T = decay_t<decltype(arg)>;
            this->add_object(new_object, arg);

            if constexpr (is_same<T, debye>::value) {
                add_polarization(P_debye, arg, new_object, grid);
                if (!prevE)
                    prevE = Ez;
            }
            else if constexpr (is_same<T, drude>::value) {
                add_polarization(P_drude, arg, new_object, grid);
                if (!prevE)
                    prevE = Ez;
            }
            else if constexpr (is_same<T, lorentz>::value) {
                add_polarization(P_lorentz, arg, new_object, grid);
                if (!prevE)
                    prevE = Ez;
                if (!prev2E)
                    prevE = Ez;
            }
            else if constexpr (is_same<T, simple_material>::value) {
                // Do nothing?
            }
        }, m);
    }

    void Field2D::add_source(source &new_source) {
        new_source.set_F(this);
        source_list.push_back(&new_source);
    }

    void Field2D::add_monitor(monitor &new_monitor) {
        new_monitor.set_F(this);
        monitor_list.push_back(&new_monitor);
    } 

    void Field2D::clear_monitors() {
        monitor_list.clear();
    }

    void Field2D::clear_fields() {
        t = 0;
        tStep = 0;
        Ez = 0*Ez; 
        Hx = 0*Ez; 
        Hy = 0*Ez; 
    }

    void Field2D::clear_materials() {
        for (int i = 0; i < Nx; i++) {
            for (int j = 0; j < Ny; j++) {
                Ca(i,j) = background->Ca(dt); 
                Cb(i,j) = background->Cb(dt); 
                Da(i,j) = background->Da(dt); 
                Db(i,j) = background->Db(dt); 
             }
        }
    }

    void Field2D::set_tfsf(const volume& vol, const time_profile& tp) {
        total = tfsf(grid, tp, vol, dt);
        //auto sources_group = outFile->create_or_open_group("sources");
        //auto group = sources_group.create_or_open_group("tfsf");
        //total->write(group);
    }

    void Field2D::set_tfsf_freq(const freq_data &freq) {
        if (total) {
            total->add_dft(freq);
        }
        auto sources_group = outFile->create_or_open_group("sources");
        auto group = sources_group.create_or_open_group("tfsf");
        freq.write(group);
    }

    void Field2D::write_tfsf() {
        if (total) {
            auto sources_group = outFile->create_or_open_group("sources");
            auto group = sources_group.create_or_open_group("tfsf");
            total->write(group);
            Array S = total->compute_flux();
            write_array<double, h5cpp::dtype::Double>(group,S,"flux");
        }
    }


    double Field2D::interpolate(fields C, const vec &p) {
        vec ps = p/dx; 
        auto &F = get_field_ref(C);

        switch(C) {
            case fields::Ez:  ps += vec(-dx/2.0, dx/2.0); break;
            case fields::Hx: ps += vec(-dx/2.0, 0); break;
            case fields::Hy: ps += vec(0, dx/2.0); break;
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

    tensor& Field2D::get_field_ref(fields F) {
        switch(F) {
            case fields::Ez:  return Ez; break; 
            case fields::Hx:  return Hx; break; 
            case fields::Hy:  return Hy; break; 
            default: throw std::invalid_argument("not a valid field component");
        }
    }

    void Field2D::create_fields_dataset(fields field) {

        vector<hsize_t> dims = {hsize_t(Nx),hsize_t(Ny),1};
        vector<hsize_t> max_dims = {hsize_t(Nx),hsize_t(Ny),h5cpp::inf};
        vector<hsize_t> chunk_dims = dims;
        h5cpp::dspace ds(dims, max_dims, chunk_dims, false);

        auto gFields = outFile->create_or_open_group("fields");
        h5cpp::h5dset dset;

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

    void Field2D::add_object(object &new_object, const material& mat) {
        auto mat_name = mat.get_name();
        if (find(materials_added.begin(), materials_added.end(), mat_name) == materials_added.end()) {
            materials_added.push_back(mat_name);
            mat.write(*outFile);
        }
        
        obj_list.push_back(&new_object);
        new_object.set_owner(this);

        update_object_material_grid(new_object);
    }

    void Field2D::update_object_material_grid(const object &obj) {
        auto mat = obj.get_material_base();
        for (int i = 0; i < Nx; i++) {
            for (int j = 0; j < Ny; j++) {
                ivec pi = {i,j};
                vec p = grid.to_real(pi);

                if (obj.inside(p)) {
                    Ca(pi) = mat->Ca(dt); 
                    Cb(pi) = mat->Cb(dt); 
                    Da(pi) = mat->Da(dt); 
                    Db(pi) = mat->Db(dt); 
                }
            }
        }
    }

}
