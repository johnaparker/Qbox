#include <vector>
#include <math.h>
#include "objects/object.h"

using namespace std;

//separate into geometry and material classes as needed
//support for mobility

namespace qbox {
    object::object() {
        eps = 1;
        conduc = 0;
    }

    void object::set_eps(double e) {
        eps = e;
    }
    void object::set_conduc(double c) {
        conduc = c;
    }
}
