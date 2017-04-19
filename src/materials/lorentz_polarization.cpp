#include "materials/lorentz_polarization.h"
#include "field2.h"

using namespace std;

namespace qbox {
    lorentz_polarization::lorentz_polarization(const grid_properties &grid, const lorentz &mat): polarization(grid, mat.Npoles()), mat(mat) {
        delta = tensor3(grid.Nx, grid.Ny, Npoles);
        prevJ = tensor3(grid.Nx, grid.Ny, Npoles);
    }

    void lorentz_polarization::insert_object(const object &new_object) {
        Array delta_vals = mat.delta(grid.dt);
        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                ivec pi = {i,j};
                vec p = grid.to_real(pi);

                if (new_object.inside(p)) {
                    for (int N = 0; N < Npoles; N++)
                        delta(i,j,N) = delta_vals[N];
                }
            }
        }
    }

    void lorentz_polarization::update_J(Field2D &f) {
        double dt = grid.dt;
        Array alpha = mat.alpha(dt);
        Array beta = mat.beta(dt);
        double Cb = mat.Cb(dt);
        double Ca_prev = mat.Ca_prev(dt);

        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                for (int N = 0; N < Npoles; N++) {
                    f.Ez(i,j) += Ca_prev*(*f.prev2E)(i,j) 
                        - Cb*0.5*((1 + alpha(N))*J(i,j,N) + beta(N)*prevJ(i,j,N));
                    double temp_J = J(i,j,N);
                    J(i,j,N) = alpha(N)*J(i,j,N) + beta(N)*prevJ(i,j,N) 
                        + delta(i,j,N)/(2*dt)*(f.Ez(i,j) - (*f.prev2E)(i,j));
                    prevJ(i,j,N) = temp_J;
                }
            }
        }
    }
}
