#ifndef GUARD_medium_h
#define GUARD_medium_h

#include <vector>
#include "object.h"

namespace qbox {
    //medium object: special object for background medium
    class medium: public object {
    public:
        medium();
        bool inside(std::vector<int> p) const override;
    };
}

#endif
