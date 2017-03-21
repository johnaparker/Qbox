#ifndef GUARD_medium_h
#define GUARD_medium_h

#include "geometry/geometry.h"

namespace qbox {
    //medium object: special object for background medium

    class medium: public geometry {
    public:
        medium();
        bool inside(const vec& v) const override;
        void write(h5cpp::h5group& group) const override {};
        std::unique_ptr<geometry> clone() const override;
    };

}


#endif
