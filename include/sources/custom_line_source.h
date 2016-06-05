#ifndef GUARD_custom_line_source_h
#define GUARD_custom_line_source_h

#include <vector>
#include "source.h"

namespace apine {
    class custom_line_source: public source {
    public:
        std::vector<int> p1, p2;
        double (*time_func)(double);
    public:
        custom_line_source(std::vector<int> p1, std::vector<int> p2, double (*time_func)(double));
        void pulse();
    };
}

#endif
