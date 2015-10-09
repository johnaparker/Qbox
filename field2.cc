#include "field2.h"
#include "matrix.h"
#include "object.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

Field2D::Field2D(grid_properties grid, double dt): Nx(grid.Nx), Ny(grid.Ny), dx(grid.dx), dt(dt) {
    Lx = Nx*dx; 
    Ly = Ny*dx; 
    t = 0;
    tStep = 0;
    Ez = matrix<double>(new double [Nx*Ny], Nx, Ny); 
    Hx = matrix<double>(new double [Nx*Ny], Nx, Ny);
    Hy = matrix<double>(new double [Nx*Ny], Nx, Ny);
    Dz = matrix<double>(new double [Nx*Ny], Nx, Ny);
    Iz = matrix<double>(new double [Nx*Ny], Nx, Ny);
    ca = matrix<double>(new double [Nx*Ny], Nx, Ny);
    cb = matrix<double>(new double [Nx*Ny], Nx, Ny);
    matrix<double> eps(new double [Nx*Ny], Nx, Ny);       //need to be deallocated
    matrix<double> conduc(new double [Nx*Ny], Nx, Ny);    //here too

    obj  = matrix<object*>(new object* [Nx*Ny], Nx, Ny); 
    obj_list = {new medium()};
    
    for (int i = 0; i != Nx; i++) {
        for (int j = 0; j != Ny; j++) {
            Ez[i][j] = 0;
            Hx[i][j] = 0;
            Hy[i][j] = 0;
            Dz[i][j] = 0;
            Iz[i][j] = 0;
            eps[i][j] = obj_list[0]->eps;      
            conduc[i][j] = obj_list[0]->conduc; 
            ca[i][j] = 1/(eps[i][j] + conduc[i][j]*dt/epsilon);
            cb[i][j] = conduc[i][j]*dt/epsilon;
            obj[i][j] = obj_list[0];
         }
    }

    BC = pml(grid); 

    if (grid.totalFieldScatteredField) 
        total = new tfsf(grid, dt);
    else
        total = nullptr;


    outE.open("Eout.dat", ios::binary);
    outH.open("Hout.dat", ios::binary);
}

void Field2D::write() {
    static int th = BC.thickness;
    for (int i = th; i!= Nx-th; i++) {
        for (int j=th; j!= Ny-th; j++) {
            outE.write(reinterpret_cast<char*>(&Ez[i][j]),sizeof(Ez[i][j]));
            //outH.write(reinterpret_cast<char*>(&Hy[k]),sizeof(Hy[k]));
        }
    }
}

void Field2D::display_info(double tf) {
    cout << setw(10) << "dx:" << dx << " m" << endl;
    cout << setw(10) << "Lx:" << dx*Nx << " m" << endl;
    cout << setw(10) << "Ly:" << dx*Ny << " m" << endl;
    cout << setw(10) << "dt:" << dt << " s" << endl;
    cout << setw(10) << "Tf:" << tf << " s" << endl;
}

void Field2D::pulse(double f) {
    static double T = 0.5e-7;
    static double sig = 1e-8;
    //double p = 0.2*exp(-0.5*(pow((t-T)/sig,2)));
    double p = 0.05*sin(2*M_PI*f*t);
    Dz[35][30] = p;
}

void Field2D::update() {
    tStep += 1;
    t += dt;
    
    for (const auto &s : source_list) {
        s->pulse();
    } 

    for (int i=1; i<Nx-1; i++) {
        for (int j=1; j<Ny-1; j++) {
            Dz[i][j] = BC.gi3[i]*BC.gj3[j]*Dz[i][j] + 
                BC.gi2[i]*BC.gj2[j]*0.5*(Hy[i][j]-Hy[i-1][j] - Hx[i][j]+Hx[i][j-1]);
         }
    }

    if (total) 
        total->updateD(this);

    for (int i=1; i<Nx-1; i++) {
        for (int j=1; j<Ny-1; j++) {
            Ez[i][j] = ca[i][j]*(Dz[i][j] -Iz[i][j]);
            Iz[i][j] += cb[i][j]*Ez[i][j];
         }
    }


    //this can possibly be moved to the previous if statement
    if (total) 
        total->pulse();

    for (int i=1; i<Nx-1; i++) {
        for (int j=1; j<Ny-1; j++) {
            double curl_e = Ez[i+1][j] - Ez[i][j];
            BC.Ihy[i][j] += curl_e;
            Hy[i][j] = BC.fi3[i]*Hy[i][j] + BC.fi2[i]*0.5*curl_e + BC.fj1[j]*BC.Ihy[i][j];
            
            curl_e = Ez[i][j] - Ez[i][j+1];
            BC.Ihx[i][j] += curl_e;
            Hx[i][j] = BC.fj3[j]*Hx[i][j] + BC.fj2[j]*0.5*curl_e + BC.fi1[i]*BC.Ihx[i][j];
        }
    }

    if (total) 
        total->updateH(this);
}

void Field2D::run(double time) {
    int count = 0;
    int saveTime = 1;
    int totSteps = round(time/dt);
    display_info(time);

    while (t < time) {
        update();
        count ++;
        if (count >= saveTime) {
            cout << tStep << "/" << totSteps << '\r';
            cout.flush();
            count = 0;
            write();
        } 
    }
    cout << tStep << "/" << totSteps << endl;

    outE.close();
    outH.close();
}


void Field2D::add_object(object &new_object) {
    obj_list.push_back(&new_object);
    double eps = new_object.eps;
    double conduc = new_object.conduc;

    for (int i = 0; i != Nx; i++) {
        for (int j = 0; j != Ny; j++) {
            if (new_object.inside(i,j)) {
                obj[i][j] = &new_object;
                obj[i][j] = &new_object;
                    
                ca[i][j] = 1/(eps + conduc*dt/epsilon);
                cb[i][j] = conduc*dt/epsilon;
            }
        }
    }
}

void Field2D::add_source(source &new_source) {
    new_source.set_F(this);
    source_list.push_back(&new_source);
}

grid_properties::grid_properties(int Nx, int Ny, double dx, int pml_thickness):
        Nx(Nx), Ny(Ny), dx(dx), pml_thickness(pml_thickness) {
            totalFieldScatteredField = false;       
}

void grid_properties::set_tfsf(vector<int> p1_val, vector<int> p2_val){
    p1 = p1_val;
    p2 = p2_val;
    totalFieldScatteredField = true;
}

void grid_properties::set_tfsf(int xbuff, int ybuff){
    vector<int> p1_val = {xbuff, ybuff};
    vector<int> p2_val = {Nx-xbuff, Ny-ybuff};
    set_tfsf(p1_val, p2_val);
}

void grid_properties::set_tfsf(int buff){
    set_tfsf(buff, buff);
}
