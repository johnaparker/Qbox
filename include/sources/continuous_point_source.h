#ifndef GUARD_continuous_point_source_h
#define GUARD_continuous_point_source_h

#include <vector>
#include "source.h"

namespace apine {
    class continuous_point_source: public source {
    public:
        continuous_point_source(std::vector<int> p, double f);
        void pulse();
    public:
        std::vector<int> p;    //vector position
        double freq;           //frequency
    };
}

#endif
