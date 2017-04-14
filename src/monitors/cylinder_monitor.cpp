#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "monitors/cylinder_monitor.h"

using namespace std;


namespace qbox {
    cylinder_monitor::cylinder_monitor(string name, const cylinder_surface &surf, const freq_data &freq, bool extendable): monitor(name,"cylinder_monitor",freq,extendable), surf(surf), length(0) {
        F = nullptr;
    }

    cylinder_monitor::cylinder_monitor(const cylinder_surface &surf, const freq_data &freq, bool extendable): monitor("cylinder_monitor",freq,extendable), surf(surf), length(0) {
        F = nullptr;
    }

    void cylinder_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);

        length = ceil(2*M_PI*surf.radius/F->dx);

        prevE = Array::Zero(length);
        rE = tensor(freq.size(), length);
        iE = tensor(freq.size(), length);
        rH = tensor(freq.size(), length);
        iH = tensor(freq.size(), length);

        auto group = get_group();
        surf.write(group);
    }

    void cylinder_monitor::update() {
        //*** this dir, Hfield combo is bad design (maybe use enum)

        double E = 0;
        double H = 0;
        freq.update(F->t);

        for (int i = 0; i < length; i++) {
            double theta = 2*M_PI*(i + 0.5)/(length + 1);
            vec pr = surf.position(theta);

            //H = -sin(theta)*F->interpolate(fields::Hx, pr) + cos(theta)*F->interpolate(fields::Hy, pr);
            H = vec(F->interpolate(fields::Hx, pr), F->interpolate(fields::Hy, pr)).dot(surf.tangent(theta));
            E = F->interpolate(fields::Ez, pr);
            E = (E + prevE(i))/2;

            prevE(i) = F->interpolate(fields::Ez, pr);

            const int max_freq = freq.size();
#pragma GCC ivdep
            for (int j = 0; j < max_freq; j++) {
                rE(j,i) += E*freq.get_cosf(j);
                iE(j,i) += E*freq.get_sinf(j);

                rH(j,i) += H*freq.get_cosf(j);
                iH(j,i) += H*freq.get_sinf(j);
            }
        }
    }

    Array cylinder_monitor::compute_flux() const {
        Array S = Array::Zero(freq.size());

        for (int j = 0; j != freq.size(); j++) {
            for (int i = 0; i != length; i++) {
                S[j] -= rE(j,i)*rH(j,i) + iE(j,i)*iH(j,i);
            }
        }
        //S *= F->dx;
        S *= 2*M_PI*surf.radius/length;
        return S;
    }

}


