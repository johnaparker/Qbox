#ifndef GUARD_gaussian_point_source_h
#define GUARD_gaussian_point_source_h

#include <vector>
#include "source.h"

namespace qbox {
    class gaussian_point_source: public source {
    public:
        gaussian_point_source(std::vector<int> p, double T0, double sig);
        void pulse();
    private:
        std::vector<int> p;    //vector position
        double T0, sig;        //temporal center, width
    };
}

#endif
