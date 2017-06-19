#ifndef GUARD_box_monitor_h
#define GUARD_box_monitor_h

#include "rank_monitor.h"
#include <vector>
#include <math.h>
#include "../field2.h"

#include "flux.h"
#include "ntff.h"
#include "force.h"

namespace qbox {
    template <class T = DFT::all>
    class box_monitor: public rank_monitor<1> {
        static constexpr char const* sub_group = "box_monitor";
    public:
        box_monitor(std::string name, const volume &vol, const Array &freq): rank_monitor(name, sub_group, freq), vol(vol) {
            Nfreq = freq.size();
            monitors[0] = surface_monitor<T>(name + "_1", vol.get_surface(box_side::y_bottom), freq);
            monitors[1] = surface_monitor<T>(name + "_2", vol.get_surface(box_side::x_top),    freq);
            monitors[2] = surface_monitor<T>(name + "_3", vol.get_surface(box_side::y_top),    freq);
            monitors[3] = surface_monitor<T>(name + "_4", vol.get_surface(box_side::x_bottom), freq);
        }

        box_monitor(const volume &vol, const Array &freq): box_monitor("monitor_" + std::to_string(_num_created), vol, freq) {_num_created++;}

        void set_F(Field2D *newF) {
            monitor::set_F(newF);
            for (int i = 0; i != 4; i++) 
                monitors[i].set_F(newF);

            auto group = get_group();
            vol.write(group);
            fourier.write_properties(group);
        }

        void update() {
            for (int i = 0; i < 4; i++) 
                monitors[i].update();
        }

        Flux flux() const {
            Array S = Array::Zero(Nfreq);

            for (int i = 0; i < 4; i++) {
                Array Sm = monitors[i].flux().data();
                S += Sm;
            }
            return Flux(S, *outFile, get_group());
        }

        ntff_point ntff(const vec &p) const {
            vec center = vol.center();
            ComplexArray result = ComplexArray::Zero(Nfreq);

            for (int i = 0; i < 4; i++)
                result += monitors[i].ntff(center, p).data();

            return ntff_point(result, *outFile, get_group());
        }

        ntff_sphere ntff(double radius, int N) const {
            ComplexTensor result(N, Nfreq);
            Array theta = Array::LinSpaced(N, 0, 2*M_PI);

            for (int i = 0; i < N; i++) {
                vec p = radius*vec(cos(theta[i]), sin(theta[i])) + vol.center();
                ComplexArray p_ntff = ntff(p).data();
                for (int j = 0; j < Nfreq; j++)
                    result(i,j) = p_ntff(j);
            }
            return ntff_sphere(result, *outFile, get_group());
        }

        Force force() const {
            tensor S(2, Nfreq); S.setZero();

            for (int i = 0; i < 4; i++) {
                tensor Sm = monitors[i].partial_force().data();
                S += Sm;
            }
            return Force(S, *outFile, get_group());
        }
        // void write() const {
        // }

        // void operator-=(const box_monitor& other) {
        //     for (int i = 0; i != 4; i++)
        //         monitors[i] -= other.monitors[i];
        // }

        //equivalent currents
        //ComplexTensor Jeq() const;
        //ComplexTensor Meq() const;

    private:
        int Nfreq;
        surface_monitor<T> monitors[4];
        volume vol;

    };

}

#endif
