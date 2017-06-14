#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "field2.h"
#include "monitors/cylinder_monitor.h"

using namespace std;


namespace qbox {
    cylinder_monitor::cylinder_monitor(string name, const cylinder_surface &surf, const Array &freq): monitor(name,"cylinder_monitor"), surf(surf), length(0), freq(freq) {
        F = nullptr;
    }

    cylinder_monitor::cylinder_monitor(const cylinder_surface &surf, const Array &freq): monitor("cylinder_monitor"), surf(surf), length(0), freq(freq) {
        F = nullptr;
    }

    void cylinder_monitor::set_F(Field2D *newF) {
        monitor::set_F(newF);

        length = ceil(2*M_PI*surf.radius/F->dx);
        fourier = dft<1>(freq, {"Ez", "H_rho"}, Array::Constant(1,length));
        prevE = Array::Zero(length);

        auto group = get_group();
        surf.write(group);
    }

    void cylinder_monitor::update() {
        //*** this dir, Hfield combo is bad design (maybe use enum)

        tensor1 E(length);
        tensor1 H(length);
        freq.update(F->t);

        for (int i = 0; i < length; i++) {
            double theta = 2*M_PI*(i + 0.5)/(length + 1);
            vec pr = surf.position(theta);
            //H = -sin(theta)*F->interpolate(fields::Hx, pr) + cos(theta)*F->interpolate(fields::Hy, pr);
            H = vec(F->interpolate(fields::Hx, pr), F->interpolate(fields::Hy, pr)).dot(surf.tangent(theta));
            E = F->interpolate(fields::Ez, pr);
            double tempE = E;
            E = (E + prevE(i))/2;
            prevE(i) = tempE;


        }
    }

    Array cylinder_monitor::compute_flux() const {
        Array S = Array::Zero(freq.size());

        for (int i = 0; i < length; i++) {
            for (int j = 0; j < freq.size(); j++) {
                S[j] -= rE(i,j)*rH(i,j) + iE(i,j)*iH(i,j);
            }
        }
        S *= 2*M_PI*surf.radius/length;
        return S;
    }

}


