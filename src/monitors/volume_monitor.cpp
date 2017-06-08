#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "field2.h"
#include "monitors/volume_monitor.h"

using namespace std;


namespace qbox {
    volume_monitor::volume_monitor(string name, const volume &vol, const freq_data &freq): monitor(name,"volume_monitor",freq, false), vol(vol) {
        F = nullptr;
        Nfreq = freq.size();
    }
    volume_monitor::volume_monitor(const volume &vol, const freq_data &freq): monitor("volume_monitor", freq, false), vol(vol) {
        F = nullptr;
        Nfreq = freq.size();
    }

    void volume_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);
        auto ivol = newF->grid.to_grid(vol);

        Nx = ivol.dim(0) + 1;
        Ny = ivol.dim(1) + 1;

        rE = tensor3(Nx, Ny, Nfreq); rE.setZero();
        iE = tensor3(Nx, Ny, Nfreq); iE.setZero();

        auto group = get_group();
        vol.write(group);
    }

    void volume_monitor::update() {
        auto ivol = F->grid.to_grid(vol);

        double E = 0;
        freq.update(F->t);

        for (int i = 0; i < Nx; i++) {
            for (int j = 0; j < Ny; j++) {
                ivec p = ivol.a + ivec(i,j);
                E = F->Ez(p);

#pragma GCC ivdep
                for (int k = 0; k < Nfreq; k++) {
                    rE(i,j,k) += E*freq.get_cosf(k);
                    iE(i,j,k) += E*freq.get_sinf(k);
                }
            }
        }
    }

    void volume_monitor::write_dft() const {
        ComplexTensor3 result(Nx,Ny,Nfreq);
        for (int i = 0; i < Nx; i++)
            for (int j = 0; j < Ny; j++)
                for (int k = 0; k < Nfreq; k++)
                    result(i,j,k) = rE(i,j,k) + 1i*iE(i,j,k);

        auto my_group = get_group();
        write_tensor<complex<double>,3,h5cpp::dtype::Complexd>(my_group, result, "dft");
    }

}
