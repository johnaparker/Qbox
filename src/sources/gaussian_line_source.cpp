#include "sources/gaussian_line_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    gaussian_line_source::gaussian_line_source(const surface& surf, double T0, double sig):
        p1(surf.a.cast<int>()), p2(surf.b.cast<int>()), T0(T0), sig(sig) {};

    void gaussian_line_source::pulse() {
    }
}
