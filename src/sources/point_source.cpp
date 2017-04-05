#include "sources/point_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    point_source::point_source(fields C, vec p, const time_profile &tp):
        source(tp), C(C), p(p) {};

    void point_source::pulse() {
        static ivec pi = (F->grid).to_grid(p);
        F->get_field_ref(C)(pi) += tp->response(*t);
    }

    void point_source::write(const h5cpp::h5group &group) {
        source::write(group);
        write_vec<double, h5cpp::dtype::Double>(group, p, "position");
        //write field component (enum?)
    }
}
