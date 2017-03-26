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
        int length = isurf.dim.norm();

        prevE = matrix<double,1>(length+1);
        rE = matrix<double,2>(length, freq.size());
        iE = matrix<double,2>(length, freq.size());
        rH = matrix<double,2>(length, freq.size());
        iH = matrix<double,2>(length, freq.size());

        for (int i = 0; i != length; i++) {
            prevE(i) = 0;
            for (int j = 0; j!= freq.size(); j++) {
                rE(i,j) = 0;
                iE(i,j) = 0;
                rH(i,j) = 0;
                iH(i,j) = 0;
            }
        }
        prevE(length) = 0;

        auto gName = get_group();
        auto dspace = h5cpp::dspace(vector<hsize_t>{2});
        auto attr = gName.create_attribute("p1", h5cpp::dtype::Double, dspace);
        attr.write(surf.a.data());
        attr = gName.create_attribute("p2", h5cpp::dtype::Double, dspace);
        attr.write(surf.b.data());
    }

    void surface_monitor::update() {
        //*** this dir, Hfield combo is bad design (maybe use enum)
        auto isurf = F->grid.to_grid(surf);
        auto *Hfield = isurf.dim[0] == 0 ? &F->Hy : &F->Hx;

        double E = 0;
        double H = 0;
        freq.update(F->t);

        //this if check could be done outside the for loop somehow
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
        prevE(length) = F->Ez(isurf.b + isurf.tangent);
    }

    Eigen::ArrayXd surface_monitor::compute_flux() const {
        Eigen::ArrayXd S = Eigen::ArrayXd::Zero(freq.size());

        for (int j = 0; j != freq.size(); j++) {
            for (int i = 0; i != length; i++) {
                S[j] += rE(i,j)*rH(i,j) + iE(i,j)*iH(i,j);
            }
        }
        S *= F->dx;
        return S;
    }

    void surface_monitor::write_flux() {
        auto S = compute_flux();
        auto gName = get_group();

        h5cpp::h5dset dset;
        if (!gName.object_exists("flux")) {
            vector<hsize_t> dims = {hsize_t(freq.size())};
            vector<hsize_t> max_dims = {hsize_t(freq.size())};
            if (!extendable) {
                h5cpp::dspace ds(dims, max_dims);
                dset = gName.create_dataset("flux", 
                             h5cpp::dtype::Double, ds); 
            }
            else {
                dims.push_back(1);
                max_dims.push_back(h5cpp::inf);
                vector<hsize_t> chunk_dims = dims;
                h5cpp::dspace ds(dims, max_dims, chunk_dims);
                dset = gName.create_dataset("flux", 
                             h5cpp::dtype::Double, ds); 
            }
            dset.write(S.data());
        }
        else {
            dset = gName.open_dataset("flux");
            dset.append(S.data());
        }
    }
}


