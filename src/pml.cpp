#include "matrix.h"
#include "field2.h"
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

//Perhaphs allow pml of varying thickness on each side.

namespace qbox {

    pml::pml(grid_properties grid, int m, int ma, double k_max, double a_max, double sig_scale):
       grid(grid), thickness(grid.pml_thickness), m(m), ma(ma), k_max(k_max), a_max(a_max) {
           sig_max = 0.8*(m+1)/grid.dx*sig_scale;     //*** include wave impedance if non-vacuum
        }

    double pml::b_func(double t) {
        return exp(-grid.dt*(sig_func(t)/k_func(t) + a_func(t)));
    }

    double pml::c_func(double t) {
        return sig_func(t)/(sig_func(t)*k_func(t) + pow(k_func(t),2)*a_func(t))*(b_func(t) - 1);
    }

    double pml::a_func(double t) {
        if (t > thickness)
            return 0;
        return a_max*pow((t-1)/(thickness-1), ma);
    }

    double pml::k_func(double t) {
        if (t > thickness)
            return 1;
        return 1 + (k_max - 1)*pow((thickness-t)/(thickness-1), m);
    }

    double pml::sig_func(double t) {
        if (t > thickness)
            return 0;
        return sig_max*pow((thickness-t)/(thickness-1), m);

    }

}
