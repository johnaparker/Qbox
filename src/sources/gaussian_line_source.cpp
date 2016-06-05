#include "sources/gaussian_line_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace qbox {

    gaussian_line_source::gaussian_line_source(vector<int> p1, vector<int> p2, double T0, double sig):
        p1(p1), p2(p2),T0(T0), sig(sig) {};

    void gaussian_line_source::pulse() {
    }
}
