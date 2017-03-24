#include "sources/continuous_line_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace qbox {

    continuous_line_source::continuous_line_source(const surface &surf, double freq): p1(surf.a.cast<int>()), p2(surf.b.cast<int>()), freq(freq) {};

    void continuous_line_source::pulse() {
        static ivec p1i = (F->grid).convertToGrid(p1);
        static ivec p2i = (F->grid).convertToGrid(p2);

        int start = 0, end = 0;
        bool vertical = false;
        if (p1i[0] == p2i[0]) {
            start = p1i[1];
            end = p2i[1];
            vertical = true;
        }
        else if (p1i[1] == p2i[1]) {
            start = p1i[0];
            end = p2i[0];
        }
        double pulse_amp = sin(2*M_PI*freq*(*t));
        for (int i = start; i != end; i++) {
            if (vertical)
                F->Ez(p1i[0],i) += pulse_amp;
            else
                F->Ez(i,p2i[1]) += pulse_amp;
        }
    }
}

