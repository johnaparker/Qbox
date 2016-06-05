#include "sources/custom_point_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace qbox {

    custom_point_source::custom_point_source(vector<int> p, double (*time_func)(double)):
        p(p), time_func(time_func) {};

    void custom_point_source::pulse() {
        static vector<int> pi = (F->grid).convertToGrid(p);
        F->Ez[pi[0]][pi[1]] += time_func(*t);
    }
}
