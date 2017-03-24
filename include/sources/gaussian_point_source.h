#ifndef GUARD_gaussian_point_source_h
#define GUARD_gaussian_point_source_h

#include "source.h"

namespace qbox {
    class gaussian_point_source: public source {
    public:
        gaussian_point_source(ivec p, double f, double sig, double T0);
        void pulse();
    private:
        ivec p;    //vector position
        double f;        //frequency center
        double  sig, T0;        //temporal center, width
    };
}

#endif
