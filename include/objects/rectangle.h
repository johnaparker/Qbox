#ifndef GUARD_rectangle_h
#define GUARD_rectangle_h

#include <vector>
#include "object.h"

namespace qbox {
    //rectangle object
    class rectangle: public object {
    public:
        rectangle(std::vector<double> pa, std::vector<double> pb);
        bool inside(std::vector<int> p);
    public:
        //*** should be volume
        std::vector<double> pa, pb;   //vector corners
    };
}

#endif
