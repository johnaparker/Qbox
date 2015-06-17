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

Field1D::Field1D(double length, double dx): L(length), dx(dx) {
    Nx = round(L/dx);
    t = 0;
    tStep = 0;
    Ex.resize(Nx,0);
    Hy.resize(Nx,0);
    epsinv.resize(Nx,1);
    for (int i = 0; i != Nx; i++) {
        epsinv[i] = (i < Nx/2) ? 1: 1/4.0;
    }

    outE.open("Eout.txt");
    outH.open("Hout.txt");
}

void Field1D::write() {
    for (int k=0; k!= Nx; k++) {
        outE << Ex[k] << endl;
        outH << Hy[k] << endl;
    }
}

void Field1D::display_info() {
    cout << setw(10) << "dx:" << dx << " m" << endl;
    cout << setw(10) << "L:" << dx*Nx << " m" << endl;
}

void Field1D::pulse(double f) {
    double p = sin(2*M_PI*f*t);
    Ex[0] = p;
}

void Field1D::update(double dt) {
    tStep += 1;
    t += dt;

    for (int k=1; k<Nx; k++) {
        Ex[k] = Ex[k] + 0.5*epsinv[k]*(Hy[k-1]-Hy[k]);
    }

    pulse(10e6);

    for (int k=0; k<Nx-1; k++) {
        Hy[k] = Hy[k] + 0.5*(Ex[k] - Ex[k+1]);
    }

    write();
}

void Field1D::run(double time, double dt) {
    int count = 0;
    int saveTime = 1000;
    int totSteps = round(time/dt);
    display_info();

    while (t < time) {
        update(dt);
        count ++;
        if (count >= saveTime) {
            cout << tStep << "/" << totSteps << '\r';
            cout.flush();
            count = 0;
        } 
    }

    outE.close();
    outH.close();
}
