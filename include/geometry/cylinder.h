#ifndef GUARD_cylinder_h
#define GUARD_cylinder_h

#include <vector>
#include "geometry.h"

namespace qbox {

    class cylinder: public geometry {
    public:
        sphere(double r);
        bool inside(const vec& v) const override;
        void write(std::unique_ptr<h5cpp::h5file>& file) const override;

    private:
        double r;
    };

}

#endif
