#ifndef GUARD_continuous_point_source_h
#define GUARD_continuous_point_source_h

#include "source.h"

namespace qbox {
    class continuous_point_source: public source {
    public:
        continuous_point_source(vec p, double f);
        void pulse();
    private:
        vec p;    //vector position
        double freq;           //frequency
    };
}

#endif
