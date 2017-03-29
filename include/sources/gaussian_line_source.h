#ifndef GUARD_gaussian_line_source_h
#define GUARD_gaussian_line_source_h

#include "source.h"

namespace qbox {
    class gaussian_line_source: public source {
    public:
        gaussian_line_source(fields C, const surface &surf, double f, double sig, double T0);
        void pulse();
    private:
        fields C;
        surface surf;
        double f, sig, T0;
    };
}

#endif
