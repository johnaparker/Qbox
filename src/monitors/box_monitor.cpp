#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "monitors/monitor.h"
#include "monitors/box_monitor.h"

using namespace std;

namespace qbox {
    box_monitor::box_monitor(string name, const volume &vol, const freq_data &freq_in, bool extendable):
                    monitor(name, freq_in, extendable), vol(vol) {
        
        monitors[0] = surface_monitor(name + "_1", surface(vol.a, vec(vol.b[0], vol.a[1])), freq, extendable);
        monitors[1] = surface_monitor(name + "_2", surface(vec(vol.b[0], vol.a[1]), vol.b), freq, extendable);
        monitors[2] = surface_monitor(name + "_3", surface(vec(vol.a[0], vol.b[1]), vol.b), freq, extendable);
        monitors[3] = surface_monitor(name + "_4", surface(vol.a, vec(vol.a[0], vol.b[1])), freq, extendable);
    }

    void box_monitor::set_freq(const freq_data &new_freq) {
        monitor::set_freq(new_freq);
        for (int i = 0; i != 4; i++) 
            monitors[i].set_freq(new_freq);
    }

    void box_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);
        for (int i = 0; i != 4; i++) 
            monitors[i].set_F(newF);

        auto gName = get_group();
        auto dspace = h5cpp::dspace(vector<hsize_t>{2});
        auto attr = gName.create_attribute("p1", h5cpp::dtype::Double, dspace);
        attr.write(vol.a.data());
        attr = gName.create_attribute("p2", h5cpp::dtype::Double, dspace);
        attr.write(vol.b.data());
    }

    void box_monitor::update() {
        for (int i = 0; i != 4; i++) 
            monitors[i].update();
    }

    Eigen::ArrayXd box_monitor::compute_flux() const {
        double sign_values[] = {-1, -1, 1, 1};
        Eigen::ArrayXd S = Eigen::ArrayXd::Zero(freq.size());

        for (int i = 0; i != 4; i++) {
            auto Sm = sign_values[i]*monitors[i].compute_flux();
            S += Sm;
        }
        return S;
    }

    void box_monitor::write_flux() {
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

    void box_monitor::write_flux_sides() {
        for (int i = 0; i != 4; i++) 
            monitors[i].write_flux();
    }
}

