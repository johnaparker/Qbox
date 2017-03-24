#include "sources/continuous_point_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace qbox {

    continuous_point_source::continuous_point_source(ivec p, double freq): p(p), freq(freq) {};

    void continuous_point_source::pulse() {
        static ivec pi = (F->grid).convertToGrid(p);
        F->Ez(pi[0],pi[1]) += sin(2*M_PI*freq*(*t));
    }
}
