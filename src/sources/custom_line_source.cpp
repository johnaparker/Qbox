#include "sources/custom_line_source.h"
#include "field2.h"
#include <math.h>
#include <vector>

using namespace std;

namespace apine {

    custom_line_source::custom_line_source(vector<int> p1, vector<int> p2, double (*time_func)(double)):
        p1(p1), p2(p2), time_func(time_func) {};

    void custom_line_source::pulse() {
    }
}
