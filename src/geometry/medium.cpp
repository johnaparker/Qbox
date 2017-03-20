#include <vector>
#include <math.h>
#include "objects/medium.h"

using namespace std;

namespace qbox {
    medium::medium() {};

    bool medium::inside(vector<int>) const {
        return true;
    }
}
