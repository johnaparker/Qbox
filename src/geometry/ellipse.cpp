#include "geometry/ellipse.h"

using namespace std;

namespace qbox {

    ellipse::ellipse(double rx, double ry): rx(rx), ry(ry) {
        set_bounding_box(vec(-rx,-ry), vec(rx,ry));
        //set_interior_box(vec(-r,-r)/sqrt(2), vec(r,r)/sqrt(2));
    };

    bool ellipse::inside(const vec& p) const{
        if (pow(p[0]/rx,2) + pow(p[1]/ry,2) < 1)
           return true;
        else return false; 
    }

    void ellipse::write(const h5cpp::h5group& group) const {
        auto dset = group.create_dataset("rx", h5cpp::dtype::Double);
        dset.write(&rx);
        dset = group.create_dataset("ry", h5cpp::dtype::Double);
        dset.write(&ry);
    }

    unique_ptr<geometry> ellipse::clone() const {
        return unique_ptr<ellipse>(new ellipse(*this));
    }


}
