#ifndef GUARD_medium_h
#define GUARD_medium_h

#include <vector>
#include "object.h"

namespace qbox {
    //medium object: special object for background medium

    class medium: public medium {
    public:
        medium();
        bool inside(const vec& v) const override;
        void write(std::unique_ptr<h5cpp::h5file>& file) const override;
    };

}

#endif
