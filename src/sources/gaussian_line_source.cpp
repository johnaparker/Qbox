#include "sources/gaussian_line_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    gaussian_line_source::gaussian_line_source(const surface& surf, double f, double sig, double T0):
        surf(surf), f(f), sig(sig), T0(T0) {};

    void gaussian_line_source::pulse() {
        static isurface isurf = (F->grid).to_grid(surf);
        double pulse_amp = sin(2*M_PI*f*(*t))*(F->grid).res*exp(-1*(pow((*t-T0)*sig*2*M_PI,2)));

        for (ivec p = isurf.a; p != isurf.b; p += isurf.tangent)
            F->Ez(p[0],p[1]) += pulse_amp;
    }

}
