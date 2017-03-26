#ifndef GUARD_custom_line_source_h
#define GUARD_custom_line_source_h

#include "source.h"

namespace qbox {
    class custom_line_source: public source {
    public:
        custom_line_source(const surface &surf, double (*time_func)(double));
        void pulse();
    private:
        surface surf;
        double (*time_func)(double);
    };
}

#endif
