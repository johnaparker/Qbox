#include "sources/gaussian_line_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    gaussian_line_source::gaussian_line_source(const surface& surf, double f, double sig, double T0):
        p1(surf.a), p2(surf.b), f(f), sig(sig), T0(T0) {};

    void gaussian_line_source::pulse() {
        static ivec p1g = (F->grid).to_grid(p1);
        static ivec p2g = (F->grid).to_grid(p2);
        static ivec unit = (p2g - p1g)/(p2g - p1g).norm();

        for (ivec p = p1g; p != p2g; p += unit)
            F->Ez(p[0],p[1]) += sin(2*M_PI*f*(*t))*(F->grid).res*exp(-1*(pow((*t-T0)*sig*2*M_PI,2)));
    }

}
