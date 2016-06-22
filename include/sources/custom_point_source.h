#ifndef GUARD_custom_point_source_h
#define GUARD_custom_point_source_h

#include <vector>
#include "source.h"

namespace qbox {
    class custom_point_source: public source {
    public:
        custom_point_source(std::vector<int> p, double (*time_func)(double));
        void pulse();
    private:
        std::vector<int> p;
        double (*time_func)(double);
    };
}

#endif
