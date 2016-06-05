#include <vector>
#include <math.h>
#include "objects/rectangle.h"

using namespace std;

namespace apine {

    rectangle::rectangle(vector<double> pa, vector<double> pb): pa(pa), pb(pb) {};

    bool rectangle::inside(vector<int> p) {
        if (p[0] < pa[0] or p[0] > pb[0] or p[1] < pa[1] or p[1] > pb[1]) 
            return false;
        else return true;
    }
}
