#include "sources/gaussian_point_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    gaussian_point_source::gaussian_point_source(fields C, vec p, double f, double sig, double T0):
        C(C), p(p), f(f), sig(sig), T0(T0) {};

    void gaussian_point_source::pulse() {
        static ivec pi = (F->grid).to_grid(p);
        F->get_field_ref(C)(pi) += sin(2*M_PI*f*(*t))*(F->grid).res*exp(-1*(pow((*t-T0)*sig*2*M_PI,2)));
    }
}
