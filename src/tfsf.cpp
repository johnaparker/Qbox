#include "tfsf.h"
#include "field2.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

//best way to implement this into sources. 

namespace qbox {

    tfsf::tfsf(std::string filename, const grid_properties &grid, const time_profile &tp, const volume &vol, double dt): filename(filename), vol(vol), tp(tp.clone()), dx(grid.dx), dt(dt), t(0) {

        auto ivol = grid.to_grid(vol);
        Nx = ivol.dim[1] + 5;
        ia = ivol.a[0];
        ib = ivol.b[0];
        ja = ivol.a[1];
        jb = ivol.b[1];

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

    void tfsf::pulse() {
        t += dt;
        double prevE = Ez[2];

        for (int k=1; k<Nx; k++) {
            Dz[k] += 0.5*(Hx[k-1]-Hx[k]);
            Ez[k] = ca[k]*(Dz[k] -Iz[k]);
            Iz[k] += cb[k]*Ez[k];
        }
        Ez[1] = tp->response(t);
        
        Ez[0] = pml.left[1];
        pml.left[1] = pml.left[0];
        pml.left[0] = Ez[1];
        
        Ez[Nx-1] = pml.right[1];
        pml.right[1] = pml.right[0];
        pml.right[0] = Ez[Nx-2];
        

        // if (dft) {
        //     freq.update(t);
        //     double E = (Ez[2] + prevE)/2.0;
        //     double H = (Hx[2] + Hx[1])/2.0;
        // }

        for (int k=0; k<Nx-1; k++) {
            Hx[k] = Hx[k] + 0.5*(Ez[k] - Ez[k+1]);
        }
    }

    void tfsf::updateD(Field2D* f) {
        for (int i = ia; i <= ib; i++) {
            //f->Dz(i,ja) += 0.5*Hx[ja-1];
            //f->Dz(i,jb) += -0.5*Hx[jb];
            f->Ez(i,ja) += 0.5*Hx[2];
            f->Ez(i,jb) += -0.5*Hx[Nx-2];
        }
    }

    void tfsf::updateH(Field2D* f) {
        for (int i = ia; i <= ib; i++) {
            //f->Hx(i,ja-1) += 0.5*Ez[ja];
            //f->Hx(i,jb) += -0.5*Ez[jb];
            f->Hx(i,ja-1) += 0.5*Ez[3];
            f->Hx(i,jb) += -0.5*Ez[Nx-2];
        }
        for (int j = ja; j <= jb; j++) {
            //f->Hy(ia-1,j) += -0.5*Ez[j];
            //f->Hy(ib,j) += 0.5*Ez[j];
            f->Hy(ia-1,j) += -0.5*Ez[j-ja+3];
            f->Hy(ib,j) += 0.5*Ez[j-ja+3];
        }
    }

    void tfsf::reset() {
        Ez.setZero();
        Dz.setZero();
        Hx.setZero();
        Iz.setZero();
        pml.left.setZero();
        pml.right.setZero();
        t = 0;

        //*** reset fourier in tp
    }

    Flux tfsf::flux() const {
        auto fourier = tp->get_dft();
        if (fourier) {
            Array S = Array::Zero(fourier->Nfreq());
            const auto E = fourier->get("dft");
            for (int j = 0; j < fourier->Nfreq(); j++) {
                S[j] += pow(E.real(j),2) + pow(E.imag(j),2);
            }
            return Flux(S, filename, "/sources/tfsf");
        }
        //throw
    }

    void tfsf::write_properties() const {
        h5cpp::h5file outFile(filename, h5cpp::io::rw);
        auto sources_group = outFile.create_or_open_group("sources");
        auto group = sources_group.create_or_open_group("tfsf");

        vol.write(group);

        auto fourier = tp->get_dft();
        if (fourier) {
            fourier->write_properties(group);
        }
    }
}
