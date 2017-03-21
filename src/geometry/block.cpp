#include "geometry/block.h"

using namespace std;

namespace qbox {

    block::block(vec dim): dim(dim) {
        set_bounding_box(-dim/2,dim/2);
        set_interior_box(-dim/2,dim/2);
    }

    bool block::inside(const vec& v) const {
        if (((v.cwiseAbs()-dim/2).array() > 0).any())
            return false;
        return true;
    }

    void block::write(h5cpp::h5group& group) const {
        auto dspace = h5cpp::dspace(vector<hsize_t>{2});
        auto attr = group.create_attribute("dimensions", h5cpp::dtype::Double, dspace);
        attr.write(dim.data());
    }

    unique_ptr<geometry> block::clone() const {
        return unique_ptr<block>(new block(*this));
    }

}
