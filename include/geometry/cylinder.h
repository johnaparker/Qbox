#ifndef GUARD_cylinder_h
#define GUARD_cylinder_h

#include "geometry/geometry.h"

namespace qbox {

    class cylinder: public geometry {
    public:
        cylinder(double r);
        bool inside(const vec& v) const override;
        void write(const h5cpp::h5group& group) const override;
        std::unique_ptr<geometry> clone() const override;

        std::string group_name() const override {return "cylinders";}

    private:
        double r;
    };

}

#endif
