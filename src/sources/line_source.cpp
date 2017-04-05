#include "sources/line_source.h"
#include "field2.h"
#include <math.h>

using namespace std;

namespace qbox {

    line_source::line_source(string name, fields C, const surface& surf, const time_profile &tp):
        source(name, "line", tp), C(C), surf(surf) {};

    line_source::line_source(fields C, const surface& surf, const time_profile &tp):
        source("line", tp), C(C), surf(surf) {};

    void line_source::pulse() {
        static isurface isurf = (F->grid).to_grid(surf);
        double pulse_amp = tp->response(*t);

        for (ivec p = isurf.a; p != isurf.b; p += isurf.tangent)
            F->get_field_ref(C)(p) += pulse_amp;
    }

    void line_source::write() {
        source::write();
        surf.write(get_group());
        //write field component (enum?)
    }

}


