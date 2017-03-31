#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "matrix.h"
#include "field2.h"
#include "monitors/cylinder_monitor.h"

using namespace std;


namespace qbox {
    cylinder_monitor::cylinder_monitor(string name, const cylinder_surface &surf, const freq_data &freq, bool extendable): monitor(name,freq,extendable), surf(surf), length(0) {
        F = nullptr;
    }

    void cylinder_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);

        length = ceil(2*M_PI*surf.radius/F->dx);

        prevE = matrix<double,1>(length);
        rE = matrix<double,2>(length, freq.size());
        iE = matrix<double,2>(length, freq.size());
        rH = matrix<double,2>(length, freq.size());
        iH = matrix<double,2>(length, freq.size());

        auto group = get_group();
        surf.write(group);
    }

    void cylinder_monitor::update() {
        //*** this dir, Hfield combo is bad design (maybe use enum)

        double E = 0;
        double H = 0;
        freq.update(F->t);

        for (int i = 0; i != length; i++) {
            double theta = 2*M_PI*(i + 0.5)/(length + 1);
            vec pr = surf.position(theta);

            //H = -sin(theta)*F->interpolate(fields::Hx, pr) + cos(theta)*F->interpolate(fields::Hy, pr);
            H = vec(F->interpolate(fields::Hx, pr), F->interpolate(fields::Hy, pr)).dot(surf.tangent(theta));
            E = F->interpolate(fields::Ez, pr);
            E = (E + prevE(i))/2;

            prevE(i) = F->interpolate(fields::Ez, pr);

            for (int j = 0; j != freq.size(); j++) {
                rE(i,j) += E*freq.get_cosf(j);
                iE(i,j) += E*freq.get_sinf(j);
                rH(i,j) += H*freq.get_cosf(j);
                iH(i,j) += H*freq.get_sinf(j);
            }
        }
    }

    Array cylinder_monitor::compute_flux() const {
        Array S = Array::Zero(freq.size());

        for (int j = 0; j != freq.size(); j++) {
            for (int i = 0; i != length; i++) {
                S[j] -= rE(i,j)*rH(i,j) + iE(i,j)*iH(i,j);
            }
        }
        //S *= F->dx;
        S *= 2*M_PI*surf.radius/length;
        return S;
    }

}


