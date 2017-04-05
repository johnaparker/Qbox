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
    Field1D::Field1D(int  Nx, double dx, double dt): Nx(Nx), dx(dx), dt(dt), L(dx*Nx), t(0), tStep(0) {

        Ez = Array::Zero(Nx);
        Hx = Array::Zero(Nx);
        Dz = Array::Zero(Nx);
        Iz = Array::Zero(Nx);
        pml.left = Eigen::Array2d::Zero();
        pml.right = Eigen::Array2d::Zero();

        Array eps = Array::Ones(Nx);
        Array conduc = Array::Zero(Nx);
        ca = 1/(eps + conduc*dt);
        cb = conduc*dt;

    }

    void Field1D::pulse(double f) {
        static double T = 80;
        static double sig = 1/200.0;
        //double p = f*exp(-0.5*(pow((t-T)/sig,2)));
        double p = sin(2*M_PI*f*t)*exp(-1*(pow((t-T)*sig*2*M_PI,2)));

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

}
