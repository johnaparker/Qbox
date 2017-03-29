#include "sources/continuous_point_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace qbox {

    continuous_point_source::continuous_point_source(fields C, vec p, double freq): C(C), p(p), freq(freq) {};

    void continuous_point_source::pulse() {
        static ivec pi = (F->grid).to_grid(p);
        F->get_field_ref(C)(pi) += sin(2*M_PI*freq*(*t));
    }
}
