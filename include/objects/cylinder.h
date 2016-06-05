#ifndef GUARD_cylinder_h
#define GUARD_cylinder_h

#include <vector>
#include "object.h"

namespace apine {

    //cylinder object
    class cylinder: public object {
    public:
        cylinder(double x, double y, double r);
        bool inside(std::vector<int> p);
    public:
        //*** should be vec
        double x,y,r;   //x,y position and radius r
    };
}

#endif
