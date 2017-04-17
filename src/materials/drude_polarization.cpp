#include "materials/drude_polarization.h"
#include "field2.h"

using namespace std;

namespace qbox {
    drude_polarization::drude_polarization(const grid_properties &grid, const drude &mat): grid(grid), mat(mat) {
        beta = tensor(grid.Nx, grid.Ny);
        J = tensor(grid.Nx, grid.Ny);
    }

    void drude_polarization::insert_object(const object &new_object) {
        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                ivec pi = {i,j};
                vec p = grid.to_real(pi);

                if (new_object.inside(p))
                    beta(i,j) = mat.beta(grid.dt);
            }
        }
    }

    void drude_polarization::update_J(Field2D &f) {
        double dt = grid.dt;
        double kappa = mat.kappa(dt);
        double Cb = mat.Cb(dt);
        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                f.Ez(i,j) -= Cb*0.5*(1 + kappa)*J(i,j);
                J(i,j) = kappa*J(i,j) + beta(i,j)*(f.Ez(i,j) + (*f.prevE)(i,j));
            }
        }
    }
}
