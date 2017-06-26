#include "geometry/cylinder.h"

using namespace std;

namespace qbox {

    cylinder::cylinder(double r): r(r) {
        set_bounding_box(vec(-r,-r), vec(r,r));
    };

    bool cylinder::inside(const vec& p) const{
        if (p.squaredNorm() < pow(r,2))
           return true;
        else return false; 
    }

    void cylinder::write(const h5cpp::h5group& group) const {
        h5cpp::write_scalar(r, group, "radius");
    }

    unique_ptr<geometry> cylinder::clone() const {
        return unique_ptr<cylinder>(new cylinder(*this));
    }

    std::optional<volume> cylinder::get_bounding_box(double theta) const {
        return geometry::get_bounding_box();
    }

}
