#include "sources/point_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    point_source::point_source(std::string name, fields C, vec p, const time_profile &tp):
        source(name, "point", tp), C(C), p(p) {};

    point_source::point_source(fields C, vec p, const time_profile &tp):
        source("point", tp), C(C), p(p) {};

    void point_source::pulse() {
        static ivec pi = (F->grid).to_grid(p);
        F->get_field_ref(C)(pi) += tp->response(*t);
    }

    void point_source::write() {
        source::write();
        write_vec<double, h5cpp::dtype::Double>(get_group(), p, "position");
        //write field component (enum?)
    }

}
