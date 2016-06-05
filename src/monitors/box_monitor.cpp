#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "h5out.h"
#include "monitors/monitor.h"
#include "monitors/box_monitor.h"

using namespace std;

namespace qbox {
    box_monitor::box_monitor(string name, vector<int> p1, vector<int> p2, shared_ptr<freq_data> freq_in, int N):
                    monitor(name, freq_in, N) {
        
        monitors[0] = move(surface_monitor(name + "_1", p1, {p2[0], p1[1]}, freq, N));
        monitors[1] = move(surface_monitor(name + "_2", {p2[0], p1[1]}, p2, freq, N));
        monitors[2] = move(surface_monitor(name + "_3", {p1[1], p2[0]}, p2, freq, N));
        monitors[3] = move(surface_monitor(name + "_4", p1, {p1[1], p2[0]}, freq, N));
    }

    box_monitor::box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double fmin, double fmax, int N):
                box_monitor(name, p1, p2, nullptr, N) {
        freq = shared_ptr<freq_data> (new freq_data(fmin, fmax, N));
        set_freq(freq);
    }

    box_monitor::box_monitor(std::string name, std::vector<int> p1, std::vector<int> p2, double f):
                box_monitor(name, p1, p2, nullptr, 1) {
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
    }

    void box_monitor::update() {
        for (int i = 0; i != 4; i++) 
            monitors[i].update();
    }

    void box_monitor::compute_flux(double *S) {
        double sign_values[] = {1, 1, -1, -1};
        double *Sm = new double[N];
        for (int i = 0; i != N; i++) {
            S[i] = 0;
            Sm[i] = 0;
        }

        for (int i = 0; i != 4; i++) {
            monitors[i].compute_flux(Sm);
            for (int j = 0; j != N; j++)
                S[j] += sign_values[i]*Sm[j];
        }
        delete[] Sm;
    }

    void box_monitor::write(string filename, bool extendable) {
        double *S = new double[N];
        compute_flux(S);
        F->write_monitor(filename, name, S, N, extendable); 
        delete[] S;
    }

    void box_monitor::write_sides(string filename, bool extendable) {
        for (int i = 0; i != 4; i++) 
            monitors[i].write(filename, extendable);
    }
}

