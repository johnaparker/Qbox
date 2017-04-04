#ifndef GUARD_ellipse_h
#define GUARD_ellipse_h

#include "geometry/geometry.h"

namespace qbox {

    class ellipse: public geometry {
    public:
        ellipse(double rx, double ry);
        bool inside(const vec& v) const override;
        void write(const h5cpp::h5group& group) const override;
        std::unique_ptr<geometry> clone() const override;

        std::string group_name() const override {return "ellipses";}

    private:
        double rx, ry;
    };

}

#endif
