#include "materials/lorentz_polarization.h"
#include "field2.h"

using namespace std;

namespace qbox {
    lorentz_polarization::lorentz_polarization(const grid_properties &grid, const lorentz &mat): polarization(grid), mat(mat) {
        delta = tensor(grid.Nx, grid.Ny);
        prevJ = tensor(grid.Nx, grid.Ny);
    }

    void lorentz_polarization::insert_object(const object &new_object) {
        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                ivec pi = {i,j};
                vec p = grid.to_real(pi);

                if (new_object.inside(p))
                    delta(i,j) = mat.delta(grid.dt);
            }
        }
    }

    void lorentz_polarization::update_J(Field2D &f) {
        double dt = grid.dt;
        double alpha = mat.alpha(dt);
        double beta = mat.beta(dt);
        double Cb = mat.Cb(dt);
        double Ca_prev = mat.Ca_prev(dt);

        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                f.Ez(i,j) += Ca_prev*(*f.prev2E)(i,j) 
                    - Cb*0.5*((1 + alpha)*J(i,j) + beta*prevJ(i,j));
                double temp_J = J(i,j);
                J(i,j) = alpha*J(i,j) + beta*prevJ(i,j) 
                    + delta(i,j)/(2*dt)*(f.Ez(i,j) - (*f.prev2E)(i,j));
                prevJ(i,j) = temp_J;
            }
        }
    }
}
