#include "materials/material.h"

using namespace std;

namespace qbox {

    int material::num_created = 0;

    h5cpp::h5group material::get_group(const h5cpp::h5file &outFile) const {
        auto mat_group = outFile.create_or_open_group("materials");
        auto my_group = mat_group.create_or_open_group(name);
        return my_group;
    }

}
