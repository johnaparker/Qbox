#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "monitors/surface_monitor.h"

using namespace std;


namespace qbox {
    surface_monitor::surface_monitor(string name, const surface &surf, shared_ptr<freq_data> freq, int N, bool extendable): monitor(name,freq,N,extendable), p1(surf.a), p2(surf.b), dir(0), length(0) {
        F = nullptr;
        prevE = nullptr;
    }

    surface_monitor::surface_monitor(string name, const surface &surf, double fmin, double fmax, int N, bool extendable): surface_monitor(name, surf, nullptr, N, extendable) {
        freq = shared_ptr<freq_data> (new freq_data(fmin, fmax, N));
    }

    surface_monitor::surface_monitor(string name, const surface &surf, double f, bool extendable): 
            surface_monitor(name, surf, nullptr, 1, extendable) {
        freq = shared_ptr<freq_data> (new freq_data(f));
    }

    void surface_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);
        p1g = (F->grid).to_grid(p1);
        p2g = (F->grid).to_grid(p2);
        dir = get_direction(p1g, p2g);
        length = p2g[dir] - p1g[dir];
        prevE = unique_ptr<double[]>(new double[length+1]);
        rE = matrix<double,2>(length, N);
        iE = matrix<double,2>(length, N);
        rH = matrix<double,2>(length, N);
        iH = matrix<double,2>(length, N);
        for (int i = 0; i != length; i++) {
            prevE[i] = 0;
            for (int j = 0; j!= N; j++) {
                rE(i,j) = 0;
                iE(i,j) = 0;
                rH(i,j) = 0;
                iH(i,j) = 0;
            }
        }
        prevE[length] = 0;

        auto gName = get_group();
        auto dspace = h5cpp::dspace(vector<hsize_t>{2});
        auto attr = gName.create_attribute("p1", h5cpp::dtype::Double, dspace);
        attr.write(p1.data());
        attr = gName.create_attribute("p2", h5cpp::dtype::Double, dspace);
        attr.write(p2.data());
    }

    void surface_monitor::update() {
        //*** this dir, Hfield combo is bad design (maybe use enum)
        matrix<double,2> *Hfield = nullptr;
        if (dir == 0)
            Hfield = &F->Hx;
        else if (dir == 1)
            Hfield = &F->Hy;

        int a = p1g[0];
        int b = p1g[1];
        double E = 0;
        double H = 0;
        (*freq).update(F->t);

        //this if check could be done outside the for loop somehow
        for (int i = 0; i != length; i++) {
            if (dir == 0) {
                a = p1g[0] + i;
                H = ((*Hfield)({a,b})+ (*Hfield)({a,b-1})
                        + (*Hfield)({a+1,b}) + (*Hfield)({a+1,b-1}))/4;
                E = (F->Ez({a,b}) + F->Ez({a+1,b})
                        + prevE[i] + prevE[i+1])/4;
            }
            else if (dir == 1) {
                b = p1g[1] + i;
                H = ((*Hfield)({a,b}) + (*Hfield)({a-1,b})
                        + (*Hfield)({a,b+1}) + (*Hfield)({a-1,b+1}))/4;
                E = (F->Ez({a,b}) + F->Ez({a,b+1})
                        + prevE[i] + prevE[i+1])/4;
            }
            prevE[i] = F->Ez({a,b});

            for (int j = 0; j != N; j++) {
                rE(i,j) += E*(*freq).get_cosf(j);
                iE(i,j) += E*(*freq).get_sinf(j);
                rH(i,j) += H*(*freq).get_cosf(j);
                iH(i,j) += H*(*freq).get_sinf(j);
            }
        }
        if (dir == 0)
            a += 1;
        else
            b += 1;
        prevE[length] = F->Ez({a,b});
    }

    unique_ptr<double[]> surface_monitor::compute_flux() const {
        auto S = make_unique<double[]>(N);
        for (int i = 0; i != N; i++) 
            S[i] = 0;

        for (int j = 0; j != N; j++) {
            for (int i = 0; i != length; i++) {
                S[j] += rE(i,j)*rH(i,j) + iE(i,j)*iH(i,j);
            }
            S[j] *= F->dx;
        }
        return S;
    }

    void surface_monitor::write_flux() {
        auto S = compute_flux();
        auto gName = get_group();

        h5cpp::h5dset dset;
        if (!gName.object_exists("flux")) {
            vector<hsize_t> dims = {hsize_t(N)};
            vector<hsize_t> max_dims = {hsize_t(N)};
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
            dset.write(S.get());
        }
        else {
            dset = gName.open_dataset("flux");
            dset.append(S.get());
        }
    }
}


