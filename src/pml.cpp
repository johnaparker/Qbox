#include "matrix.h"
#include "field2.h"
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

//Perhaphs allow pml of varying thickness on each side.

namespace qbox {

    pml::pml(grid_properties grid, int m, int ma, double k_max, double a_max, double sig_scale):
       grid(grid), thickness(grid.pml_thickness), m(m), ma(ma), k_max(k_max), a_max(a_max) {

            sig_max = 0.8*(m+1)/grid.dx*sig_scale;     //*** include wave impedance if non-vacuum

            // iniaitialize all 1-d arrays for pml
            be_x = Array::Zero(grid.Nx);
            ce_x = Array::Zero(grid.Nx);
            bh_x = Array::Zero(grid.Nx);
            ch_x = Array::Zero(grid.Nx);

            be_y = Array::Zero(grid.Ny);
            ce_y = Array::Zero(grid.Ny);
            bh_y = Array::Zero(grid.Ny);
            ch_y = Array::Zero(grid.Ny);

            for (int i = 0; i < grid.pml_thickness; i++) {
                be_x(i) = b_func(i);
                ce_x(i) = c_func(i);
                bh_x(i) = b_func(i + 0.5);
                ch_x(i) = c_func(i + 0.5);

                be_x(grid.Nx-1-i) = b_func(i);
                ce_x(grid.Nx-1-i) = c_func(i);
                bh_x(grid.Nx-1-i) = b_func(i - 0.5);
                ch_x(grid.Nx-1-i) = c_func(i - 0.5);

                be_y(i) = b_func(i);
                ce_y(i) = c_func(i);
                bh_y(i) = b_func(i + 0.5);
                ch_y(i) = c_func(i + 0.5);

                be_y(grid.Ny-1-i) = b_func(i);
                ce_y(grid.Ny-1-i) = c_func(i);
                bh_y(grid.Ny-1-i) = b_func(i - 0.5);
                ch_y(grid.Ny-1-i) = c_func(i - 0.5);
            }

            // iniaitialize all 2-d arrays for pml
            psi_Ezx1 = tensor(grid.pml_thickness+1, grid.Ny);
            psi_Ezx2 = tensor(grid.pml_thickness+1, grid.Ny);
            psi_Hyx1 = tensor(grid.pml_thickness+1, grid.Ny);
            psi_Hyx2 = tensor(grid.pml_thickness+1, grid.Ny);

            psi_Ezy1 = tensor(grid.Nx, grid.pml_thickness+1);
            psi_Ezy2 = tensor(grid.Nx, grid.pml_thickness+1);
            psi_Hxy1 = tensor(grid.Nx, grid.pml_thickness+1);
            psi_Hxy2 = tensor(grid.Nx, grid.pml_thickness+1);

        }

    double pml::b_func(double t) {
        return exp(-grid.dt*(sig_func(t)/k_func(t) + a_func(t)));
    }

    double pml::c_func(double t) {
        return sig_func(t)/(sig_func(t)*k_func(t) + pow(k_func(t),2)*a_func(t))*(b_func(t) - 1);
    }

    double pml::a_func(double t) {
        if (t > thickness)
            return 0;
        return a_max*pow((t-1)/(thickness-1), ma);
    }

    double pml::k_func(double t) {
        if (t > thickness)
            return 1;
        return 1 + (k_max - 1)*pow((thickness-t)/(thickness-1), m);
    }

    double pml::sig_func(double t) {
        if (t > thickness)
            return 0;
        return sig_max*pow((thickness-t)/(thickness-1), m);

    }

    void pml::set_scaling_factors(Array &kedx, Array &khdx, Array &kedy, Array &khdy) {

        kedx = grid.dx*Array::Ones(grid.Nx);
        khdx = grid.dx*Array::Ones(grid.Nx);
        kedy = grid.dx*Array::Ones(grid.Ny);
        khdy = grid.dx*Array::Ones(grid.Ny);

        for (int i = 0; i < grid.pml_thickness; i++) {
            kedx(i) = k_func(i)*grid.dx;
            khdx(i) = k_func(i + 0.5)*grid.dx;
            kedx(grid.Nx-1-i) = k_func(i)*grid.dx;
            khdx(grid.Nx-1-i) = k_func(i - 0.5)*grid.dx;

            kedy(i) = k_func(i)*grid.dx;
            khdy(i) = k_func(i + 0.5)*grid.dx;
            kedy(grid.Ny-1-i) = k_func(i)*grid.dx;
            khdy(grid.Ny-1-i) = k_func(i - 0.5)*grid.dx;
        }
    }

    void pml::update_E(Field2D &f) {

        for (int j=1; j<= thickness; j++) {
#pragma GCC ivdep
            for (int i=1; i<grid.Nx-1; i++) {
                psi_Ezy1(i,j) = be_y(j)*psi_Ezy1(i,j)
                                + ce_y(j)*(f.Hx(i,j) - f.Hx(i,j-1))/grid.dx;

                psi_Ezy2(i,j) = be_y(grid.Ny-2-thickness+j)*psi_Ezy2(i,j)
                                + ce_y(grid.Ny-2-thickness+j)*(f.Hx(i,grid.Ny-2-thickness+j)
                                - f.Hx(i,grid.Ny-2-thickness+j-1))/grid.dx;

                f.Ez(i,j) -= f.Cb(i,j)*psi_Ezy1(i,j);
                f.Ez(i,grid.Ny-2-thickness+j) -= f.Cb(i,grid.Ny-2-thickness+j)*psi_Ezy2(i,j);
            }
        }

        for (int j=1; j<= grid.Ny-1; j++) {
#pragma GCC ivdep
            for (int i=1; i<= thickness; i++) {
                psi_Ezx1(i,j) = be_x(i)*psi_Ezx1(i,j)
                                + ce_x(i)*(f.Hy(i,j) - f.Hy(i-1,j))/grid.dx;

                psi_Ezx2(i,j) = be_x(grid.Nx-2-thickness+i)*psi_Ezx2(i,j)
                                + ce_x(grid.Nx-2-thickness+i)*(f.Hy(grid.Nx-2-thickness+i,j)
                                - f.Hy(grid.Nx-2-thickness+i-1,j))/grid.dx;

                f.Ez(i,j) += f.Cb(i,j)*psi_Ezx1(i,j);
                f.Ez(grid.Nx-2-thickness+i,j) += f.Cb(grid.Nx-2-thickness+i,j)*psi_Ezx2(i,j);
            }
        }
    }

    void pml::update_H(Field2D &f) {
        for (int j=1; j<= thickness; j++) {
#pragma GCC ivdep
            for (int i=1; i<=grid.Nx-1; i++) {
                psi_Hxy1(i,j) = bh_y(j)*psi_Hxy1(i,j)
                                + ch_y(j)*(f.Ez(i,j+1) - f.Ez(i,j))/grid.dx;
                psi_Hxy2(i,j) = bh_y(grid.Ny-2-thickness+j)*psi_Hxy2(i,j)
                                + ch_y(grid.Ny-2-thickness+j)*(f.Ez(i,grid.Ny-2-thickness+j+1)
                                - f.Ez(i,grid.Ny-2-thickness+j))/grid.dx;

                f.Hx(i,j) -= f.Db(i,j)*psi_Hxy1(i,j);
                f.Hx(i,grid.Ny-2-thickness+j) -= f.Db(i,grid.Ny-2-thickness+j)*psi_Hxy2(i,j);
            }
        }

        for (int j=1; j<= grid.Ny-1; j++) {
#pragma GCC ivdep
            for (int i=1; i<= thickness; i++) {
                psi_Hyx1(i,j) = bh_x(i)*psi_Hyx1(i,j)
                                + ch_x(i)*(f.Ez(i+1,j) - f.Ez(i,j))/grid.dx;
                psi_Hyx2(i,j) = bh_x(grid.Nx-2-thickness+i)*psi_Hyx2(i,j)
                                + ch_x(grid.Nx-2-thickness+i)*(f.Ez(grid.Nx-2-thickness+i+1,j)
                                - f.Ez(grid.Nx-2-thickness+i,j))/grid.dx;

                f.Hy(i,j) += f.Db(i,j)*psi_Hyx1(i,j);
                f.Hy(grid.Nx-2-thickness+i,j) += f.Db(grid.Nx-2-thickness+i,j)*psi_Hyx2(i,j);
            }
        }
    }

}
