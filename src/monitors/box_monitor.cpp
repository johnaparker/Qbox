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
    box_monitor::box_monitor(string name, const volume &vol, shared_ptr<freq_data> freq_in, int N, bool extendable):
                    monitor(name, freq_in, N, extendable), p1(vol.a.cast<int>()), p2(vol.b.cast<int>()) {
        
        monitors[0] = surface_monitor(name + "_1", surface(p1.cast<double>(), vec(p2[0], p1[1])), freq, N, extendable);
        monitors[1] = surface_monitor(name + "_2", surface(vec(p2[0], p1[1]), p2.cast<double>()), freq, N, extendable);
        monitors[2] = surface_monitor(name + "_3", surface(vec(p1[0], p2[1]), p2.cast<double>()), freq, N, extendable);
        monitors[3] = surface_monitor(name + "_4", surface(p1.cast<double>(), vec(p1[0], p2[1])), freq, N, extendable);
    }

    box_monitor::box_monitor(std::string name, const volume &vol, double fmin, double fmax, int N, bool extendable):
                box_monitor(name, vol, nullptr, N, extendable) {
        freq = shared_ptr<freq_data> (new freq_data(fmin, fmax, N));
        set_freq(freq);
    }

    box_monitor::box_monitor(std::string name, const volume &vol, double f, bool extendable):
                box_monitor(name, vol, nullptr, 1, extendable) {
        freq = shared_ptr<freq_data>(new freq_data(f));
        set_freq(freq);
    }

    void box_monitor::set_freq(shared_ptr<freq_data> new_freq) {
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
        auto attr = gName.create_attribute("p1", h5cpp::dtype::Int, dspace);
        attr.write(p1.data());
        attr = gName.create_attribute("p2", h5cpp::dtype::Int, dspace);
        attr.write(p2.data());
    }

    void box_monitor::update() {
        for (int i = 0; i != 4; i++) 
            monitors[i].update();
    }

    unique_ptr<double[]> box_monitor::compute_flux() const {
        double sign_values[] = {-1, -1, 1, 1};
        auto S = make_unique<double[]>(N);
        for (int i = 0; i != N; i++) {
            S[i] = 0;
        }

        for (int i = 0; i != 4; i++) {
            auto Sm = monitors[i].compute_flux();
            for (int j = 0; j != N; j++)
                S[j] += sign_values[i]*Sm[j];
        }
        return S;
    }

    void box_monitor::write_flux() {
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

    void box_monitor::write_flux_sides() {
        for (int i = 0; i != 4; i++) 
            monitors[i].write_flux();
    }
}

