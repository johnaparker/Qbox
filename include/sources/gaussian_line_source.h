#ifndef GUARD_gaussian_line_source_h
#define GUARD_gaussian_line_source_h

#include "source.h"

namespace qbox {
    class gaussian_line_source: public source {
    public:
        gaussian_line_source(const surface &surf, double T0, double sig);
        void pulse();
    private:
        ivec p1, p2;
        double T0, sig;
    };
}

#endif
