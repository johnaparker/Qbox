#include "sources/gaussian_point_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace qbox {

    gaussian_point_source::gaussian_point_source(vector<int> p,double f, double sig, double T0):
        p(p), f(f), sig(sig), T0(T0) {};

    void gaussian_point_source::pulse() {
        static vector<int> pi = (F->grid).convertToGrid(p);
        //F->Ez[pi[0]][pi[1]] += sin(2*M_PI*f*(*t))*(F->grid).res*exp(-0.5*(pow((*t-T0)/sig,2)));
        F->Ez[pi[0]][pi[1]] += sin(2*M_PI*f*(*t))*(F->grid).res*exp(-1*(pow((*t-T0)*sig*2*M_PI,2)));
    }
}
