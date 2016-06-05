#include <vector>
#include <math.h>
#include "object.h"

using namespace std;

//separate into geometry and material classes as needed
//support for mobility

namespace apine {

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

    bool medium::inside(vector<int> p) {
        return true;
    }



    cylinder::cylinder(double x, double y, double r): x(x), y(y), r(r) {};

    bool cylinder::inside(vector<int> p){
        if (pow(p[0]-x,2) + pow(p[1]-y,2) < pow(r,2))
           return true;
        else return false; 
    }


    rectangle::rectangle(vector<double> pa, vector<double> pb): pa(pa), pb(pb) {};

    bool rectangle::inside(vector<int> p) {
        if (p[0] < pa[0] or p[0] > pb[0] or p[1] < pa[1] or p[1] > pb[1]) 
            return false;
        else return true;
    }
}
