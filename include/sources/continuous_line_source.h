#ifndef GUARD_continuous_line_source_h
#define GUARD_continuous_line_source_h

#include <vector>
#include "source.h"

namespace apine {
    class continuous_line_source: public source {
    public:
        std::vector<int> p1, p2;
        double freq;
    public:
        continuous_line_source(std::vector<int> p1, std::vector<int> p2, double f);
        void pulse();
    };
}

#endif
