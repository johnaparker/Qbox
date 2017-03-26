#include "sources/custom_point_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    custom_point_source::custom_point_source(vec p, double (*time_func)(double)):
        p(p), time_func(time_func) {};

    void custom_point_source::pulse() {
        static ivec pi = (F->grid).to_grid(p);
        F->Ez(pi[0],pi[1]) += time_func(*t);
    }
}
