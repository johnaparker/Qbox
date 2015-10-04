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

Field1D::Field1D(double length, double dx, double dt): L(length), dx(dx), dt(dt) {
    Nx = round(L/dx);
    t = 0;
    tStep = 0;
    Ex = new double [Nx];
    Hy = new double [Nx];
    Dx = new double [Nx];
    Ix = new double [Nx];
    ca = new double[Nx];
    cb = new double[Nx];

    double* eps = new double [Nx];
    double* conduc = new double [Nx];
    
    for (int i = 0; i != Nx; i++) {
        Ex[i] = 0;
        Hy[i] = 0;
        Dx[i] = 0;
        Ix[i] = 0;
        eps[i] = (i < Nx/2) ? 1: 1/1.0;
        conduc[i] = (i < Nx/2) ? 0: .0007;
        ca[i] = 1/(eps[i] + conduc[i]*dt/epsilon);
        cb[i] = conduc[i]*dt/epsilon;
    }
    pml.left[0] = 0;
    pml.left[1] = 0;
    pml.right[0] = 0;
    pml.right[1] = 0;

    delete[] eps,conduc;

    outE.open("Eout.dat", ios::binary);
    outH.open("Hout.dat", ios::binary);
}

void Field1D::write() {
    for (int k=0; k!= Nx; k++) {
        outE.write(reinterpret_cast<char*>(&Ex[k]),sizeof(Ex[k]));
        outH.write(reinterpret_cast<char*>(&Hy[k]),sizeof(Hy[k]));
    }
}

void Field1D::display_info() {
    cout << setw(10) << "dx:" << dx << " m" << endl;
    cout << setw(10) << "L:" << dx*Nx << " m" << endl;
}

void Field1D::pulse(double f) {
    static double T = .1e-6;
    static double sig = 1e-8;
    double p = exp(-0.5*(pow((t-T)/sig,2)));
    //double p = sin(2*M_PI*f*t);
    Dx[50] += p;
}

void Field1D::update() {
    tStep += 1;
    t += dt;

    for (int k=1; k<Nx; k++) {
        Dx[k] += 0.5*(Hy[k-1]-Hy[k]);
        Ex[k] = ca[k]*(Dx[k] -Ix[k]);
        Ix[k] += cb[k]*Ex[k];
    }

    pulse(10e6);

    for (int k=0; k<Nx-1; k++) {
        Hy[k] = Hy[k] + 0.5*(Ex[k] - Ex[k+1]);
    }

    pml.left[1] = pml.left[0];
    pml.left[0] = Ex[1];
    Ex[0] = pml.left[1];
    
    pml.right[1] = pml.right[0];
    pml.right[0] = Ex[Nx-2];
    Ex[Nx-1] = pml.right[1];
}

void Field1D::run(double time) {
    int count = 0;
    int saveTime = 1;
    int totSteps = round(time/dt);
    display_info();

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
