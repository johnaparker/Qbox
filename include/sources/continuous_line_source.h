#ifndef GUARD_continuous_line_source_h
#define GUARD_continuous_line_source_h

#include "source.h"
#include "vec.h"

namespace qbox {
    class continuous_line_source: public source {
    public:
        continuous_line_source(fields C, const surface &surf, double f);
        void pulse();
    private:
        fields C;
        surface surf;
        double freq;
    };
}

#endif
