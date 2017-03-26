#include "sources/custom_line_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    custom_line_source::custom_line_source(const surface &surf, double (*time_func)(double)):
        surf(surf), time_func(time_func) {};

    void custom_line_source::pulse() {
        static isurface isurf = (F->grid).to_grid(surf);
        double pulse_amp = time_func(*t);

        for (ivec p = isurf.a; p != isurf.b; p += isurf.tangent)
            F->Ez(p[0],p[1]) += pulse_amp;
    }
}
