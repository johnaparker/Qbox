#ifndef GUARD_rectangle_h
#define GUARD_rectangle_h

#include <vector>
#include "geometry.h"

namespace qbox {
    //block object
    class block: public geometry {
    public:
        block(vec dim);
        bool inside(const vec& v) const override;

    private:
        vec dim;
    };
}

#endif
