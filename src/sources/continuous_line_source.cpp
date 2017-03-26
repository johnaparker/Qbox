#include "sources/continuous_line_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace qbox {

    continuous_line_source::continuous_line_source(const surface &surf, double freq): surf(surf), freq(freq) {};

    void continuous_line_source::pulse() {
        static isurface isurf = (F->grid).to_grid(surf);
        double pulse_amp = sin(2*M_PI*freq*(*t));

        for (ivec p = isurf.a; p != isurf.b; p += isurf.tangent)
            F->Ez(p[0],p[1]) += pulse_amp;
    }
}

