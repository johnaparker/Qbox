#include "materials/drude_polarization.h"
#include "field2.h"

using namespace std;

namespace qbox {
    drude_polarization::drude_polarization(const grid_properties &grid, const drude &mat): polarization(grid, mat.Npoles()), mat(mat) {
        beta = tensor3(grid.Nx, grid.Ny, Npoles); beta.setZero();
    }

    void drude_polarization::insert_object(const object &new_object) {
        Array beta_vals = mat.beta(grid.dt);
        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                ivec pi = {i,j};
                vec p = grid.to_real(pi);

                if (new_object.inside(p)) {
                    for (int N = 0; N < Npoles; N++)
                        beta(i,j,N) = beta_vals[N];
                }
            }
        }
    }

    void drude_polarization::update_J(Field2D &f) {
        double dt = grid.dt;
        Array kappa = mat.kappa(dt);
        double Cb = mat.Cb(dt);
        for (int i = 0; i < grid.Nx; i++) {
            for (int j = 0; j < grid.Ny; j++) {
                for (int N = 0; N < Npoles; N++) {
                    f.Ez(i,j) -= Cb*0.5*(1 + kappa(N))*J(i,j,N);
                    J(i,j,N) = kappa(N)*J(i,j,N) + beta(i,j,N)*(f.Ez(i,j) + (*f.prevE)(i,j));
                }
            }
        }
    }

    void drude_polarization::reset() {
        polarization::reset();
        beta.setZero();
    }

}
