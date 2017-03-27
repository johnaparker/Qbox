#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "monitors/surface_monitor.h"

using namespace std;


namespace qbox {
    surface_monitor::surface_monitor(string name, const surface &surf, const freq_data &freq, bool extendable): monitor(name,freq,extendable), surf(surf), dir(0), length(0) {
        F = nullptr;
    }

    void surface_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);
        auto isurf = newF->grid.to_grid(surf);
        length = isurf.dim.norm();

        prevE = matrix<double,1>(length+1);
        rE = matrix<double,2>(length, freq.size());
        iE = matrix<double,2>(length, freq.size());
        rH = matrix<double,2>(length, freq.size());
        iH = matrix<double,2>(length, freq.size());

        auto group = get_group();
        surf.write(group);
    }

    void surface_monitor::update() {
        //*** this dir, Hfield combo is bad design (maybe use enum)
        auto isurf = F->grid.to_grid(surf);
        auto *Hfield = isurf.dim[0] == 0 ? &F->Hy : &F->Hx;

        double E = 0;
        double H = 0;
        freq.update(F->t);

        int i = 0;
        for (ivec p = isurf.a; p != isurf.b; p += isurf.tangent) {
            H = ((*Hfield)(p)+ (*Hfield)(p - isurf.normal)
                    + (*Hfield)(p + isurf.tangent) + (*Hfield)(p - isurf.normal + isurf.tangent))/4;
            E = (F->Ez(p) + F->Ez(p + isurf.tangent)
                    + prevE(i) + prevE(i+1))/4;

            prevE(i) = F->Ez(p);

            for (int j = 0; j != freq.size(); j++) {
                rE(i,j) += E*freq.get_cosf(j);
                iE(i,j) += E*freq.get_sinf(j);
                rH(i,j) += H*freq.get_cosf(j);
                iH(i,j) += H*freq.get_sinf(j);
            }
            i += 1;
        }
        prevE(length) = F->Ez(isurf.b);
    }

    Array surface_monitor::compute_flux() const {
        Array S = Array::Zero(freq.size());

        for (int j = 0; j != freq.size(); j++) {
            for (int i = 0; i != length; i++) {
                S[j] += rE(i,j)*rH(i,j) + iE(i,j)*iH(i,j);
            }
        }
        S *= F->dx;
        return S;
    }

}


