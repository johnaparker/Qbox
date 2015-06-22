#include "field2.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

Field2D::Field2D(double dim[], double dx, double dt): Lx(dim[0]), Ly(dim[1]), dx(dx), dt(dt) {
    Nx = round(Lx/dx);
    Ny = round(Ly/dx);
    t = 0;
    tStep = 0;
    Ez = new double [Nx][Ny];
    Hx = new double [Nx][Ny];
    Hy = new double [Nx][Ny];
    Dz = new double [Nx][Ny];
    Iz = new double [Nx][Ny];
    ca = new double[Nx][Ny];
    cb = new double[Nx][Ny];
    double* eps = new double [Nx][Ny];
    double* conduc = new double [Nx][Ny];
    
    for (int i = 0; i != Nx; i++) {
        for (int j = 0; j != Ny; j++) {
            Ez[i][j] = 0;
            Hx[i][j] = 0;
            Hy[i][j] = 0;
            Dz[i][j] = 0;
            Iz[i][j] = 0;
            eps[i][j] = (i < Nx/2) ? 1: 1/1.0;      // FIX
            conduc[i][j] = (i < Nx/2) ? 0: 0;   // FIX
            ca[i][j] = 1/(eps[i][j] + conduc[i][j]*dt/epsilon);
            cb[i][j] = conduc[i][j]*dt/epsilon;
         }
    }
    delete[] eps,conduc;

    outE.open("Eout.dat", ios::binary);
    outH.open("Hout.dat", ios::binary);
}

void Field2D::write() {
    for (int i=0; i!= Nx; i++) {
        for (int j=0; j!= Ny; j++) {
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
    static double T = .1e-6;
    static double sig = 1e-8;
    double p = exp(-0.5*(pow((t-T)/sig,2)));
    //double p = sin(2*M_PI*f*t);
    Dz[100][100] += p;
}

void Field2D::update() {
    tStep += 1;
    t += dt;

    for (int i=1; i<Nx; i++) {
        for (int j=1; j<Ny; j++) {
            Dz[i][j] += 0.5*(Hy[i][j]-Hy[i-1][j]
                            -Hx[i][j]+Hx[i][j-1]);
            Ez[i][j] = ca[i][j]*(Dz[i][j] -Iz[i][j]);
            Iz[i][j] += cb[i][j]*Ez[i][j];
         }
    }

    pulse(10e6);

    for (int i=0; i<Nx-1; i++) {
        for (int j=0; j<Ny-1; j++) {
            Hx[i][j] +=  0.5*(Ez[i][j] - Ez[i][j+1]);
            Hy[i][j] +=  0.5*(Ez[i+1][j] - Ez[i][j]);
        }
    }
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
