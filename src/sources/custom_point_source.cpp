#include "sources/custom_point_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    custom_point_source::custom_point_source(fields C, vec p, double (*time_func)(double)):
        C(C), p(p), time_func(time_func) {};

    void custom_point_source::pulse() {
        static ivec pi = (F->grid).to_grid(p);
        F->get_field_ref(C)(pi) += time_func(*t);
    }
}
