#include "sources/custom_line_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    custom_line_source::custom_line_source(const surface &surf, double (*time_func)(double)):
        p1(surf.a), p2(surf.b), time_func(time_func) {};

    void custom_line_source::pulse() {
    }
}
