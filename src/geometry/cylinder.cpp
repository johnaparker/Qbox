#include "geometry/cylinder.h"

using namespace std;

namespace qbox {

    cylinder::cylinder(double r): r(r) {
        set_bounding_box(vec(-r,-r), vec(r,r));
        set_interior_box(vec(-r,-r)/sqrt(2), vec(r,r)/sqrt(2));
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


}
