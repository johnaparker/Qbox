#ifndef GUARD_continuous_line_source_h
#define GUARD_continuous_line_source_h

#include "source.h"
#include "vec.h"

namespace qbox {
    class continuous_line_source: public source {
    public:
        continuous_line_source(const surface &surf, double f);
        void pulse();
    private:
        ivec p1, p2;
        double freq;
    };
}

#endif
