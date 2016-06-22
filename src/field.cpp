#include "field.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

namespace qbox {
    Field1D::Field1D(int  Nx, double dx, double dt): Nx(Nx), dx(dx), dt(dt) {
        L = dx*Nx;
        t = 0;
        tStep = 0;
        Ez = new double [Nx];
        Hx = new double [Nx];
        Dz = new double [Nx];
        Iz = new double [Nx];
        ca = new double[Nx];
        cb = new double[Nx];

        double* eps = new double [Nx];
        double* conduc = new double [Nx];
        
        for (int i = 0; i != Nx; i++) {
            Ez[i] = 0;
            Hx[i] = 0;
            Dz[i] = 0;
            Iz[i] = 0;
            eps[i] = (i < Nx/2) ? 1: 1/1.0;
            conduc[i] = (i < Nx/2) ? 0: .0000;
            ca[i] = 1/(eps[i] + conduc[i]*dt/epsilon);
            cb[i] = conduc[i]*dt/epsilon;
        }
        pml.left[0] = 0;
        pml.left[1] = 0;
        pml.right[0] = 0;
        pml.right[1] = 0;

        delete[] eps;
        delete[] conduc;

        outE.open("Eout.dat", ios::binary);
        outH.open("Hout.dat", ios::binary);
    }

    void Field1D::write() {
        for (int k=0; k!= Nx; k++) {
            outE.write(reinterpret_cast<char*>(&Ez[k]),sizeof(Ez[k]));
            outH.write(reinterpret_cast<char*>(&Hx[k]),sizeof(Hx[k]));
        }
    }

    void Field1D::display_info() {
        cout << setw(10) << "dx:" << dx << " m" << endl;
        cout << setw(10) << "L:" << dx*Nx << " m" << endl;
    }

    void Field1D::pulse(double f) {
        static double T = 1e-7;
        static double sig = 1e-8;
        double p = f*exp(-0.5*(pow((t-T)/sig,2)));
        //double p = sin(2*M_PI*f*t);
        Dz[10] += p;
    }

    void Field1D::update() {
        tStep += 1;
        t += dt;

        for (int k=1; k<Nx; k++) {
            Dz[k] += 0.5*(Hx[k-1]-Hx[k]);
            Ez[k] = ca[k]*(Dz[k] -Iz[k]);
            Iz[k] += cb[k]*Ez[k];
        }
        
        Ez[0] = pml.left[1];
        pml.left[1] = pml.left[0];
        pml.left[0] = Ez[1];
        
        Ez[Nx-1] = pml.right[1];
        pml.right[1] = pml.right[0];
        pml.right[0] = Ez[Nx-2];
        
        for (int k=0; k<Nx-1; k++) {
            Hx[k] = Hx[k] + 0.5*(Ez[k] - Ez[k+1]);
        }

    }

    void Field1D::run(double time) {
        int totSteps = round(time/dt);
        int saveTime = 1;
        display_info();

        int count = 0;
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
}
