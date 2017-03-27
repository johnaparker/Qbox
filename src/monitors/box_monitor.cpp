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

        auto group = get_group();
        vol.write(group);
    }

    void box_monitor::update() {
        for (int i = 0; i != 4; i++) 
            monitors[i].update();
    }

    Array box_monitor::compute_flux() const {
        double sign_values[] = {-1, -1, 1, 1};
        Array S = Array::Zero(freq.size());

        for (int i = 0; i != 4; i++) {
            auto Sm = sign_values[i]*monitors[i].compute_flux();
            S += Sm;
        }
        return S;
    }

    void box_monitor::write_flux_sides() {
        for (int i = 0; i != 4; i++) 
            monitors[i].write_flux();
    }
}

