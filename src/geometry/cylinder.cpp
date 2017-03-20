#include <vector>
#include <math.h>
#include "objects/cylinder.h"

using namespace std;

namespace qbox {
    cylinder::cylinder(double x, double y, double r): x(x), y(y), r(r) {};

    bool cylinder::inside(vector<int> p) const{
        if (pow(p[0]-x,2) + pow(p[1]-y,2) < pow(r,2))
           return true;
        else return false; 
    }
}
