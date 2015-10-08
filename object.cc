#include <vector>
#include <math.h>
#include "object.h"

using namespace std;

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



medium::medium() {};

bool medium::inside(double x, double y) {
    return true;
}



cylinder::cylinder(double x, double y, double r): x(x), y(y), r(r) {};

bool cylinder::inside(double xp, double yp){
    if (pow(xp-x,2) + pow(yp-y,2) < pow(r,2))
       return true;
    else return false; 
}


rectangle::rectangle(vector<double> pa, vector<double> pb): pa(pa), pb(pb) {};

bool rectangle::inside(double xp, double yp) {
    if (xp < pa[0] or xp > pb[0] or yp < pa[1] or yp > pb[1]) 
        return false;
    else return true;
}
