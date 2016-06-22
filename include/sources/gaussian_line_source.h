#ifndef GUARD_gaussian_line_source_h
#define GUARD_gaussian_line_source_h

#include <vector>
#include "source.h"

namespace qbox {
    class gaussian_line_source: public source {
    public:
        gaussian_line_source(std::vector<int> p1, std::vector<int> p2, double T0, double sig);
        void pulse();
    private:
        std::vector<int> p1, p2;
        double T0, sig;
    };
}

#endif
