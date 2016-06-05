#include "sources/gaussian_point_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace apine {

    gaussian_point_source::gaussian_point_source(vector<int> p, double T0, double sig):
        p(p), T0(T0), sig(sig) {};

    void gaussian_point_source::pulse() {
        static vector<int> pi = (F->grid).convertToGrid(p);
        F->Ez[pi[0]][pi[1]] += (F->grid).res*exp(-0.5*(pow((*t-T0)/sig,2)));
    }
}
