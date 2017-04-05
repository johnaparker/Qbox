#include "sources/source.h"
#include "field2.h"
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

//generalization: allow Ex,Hx,Hy, etc. excitations
//          look into this using an enum
//add TFSF support
//reduce code copying
//  maybe have line/point classes inherent from source, and then further class inherent from these
//  and use functions for time profiles
//best way to orient the line
//
//Allow amplitude specfications; allow gradual turn on for sine
//For Gaussian, get rid of T0, in favor of starting it at specified time

namespace qbox {

    source::source(const time_profile &_tp): tp(_tp.clone()) {
        F = nullptr;
        t = nullptr;
    }

    void source::set_F(Field2D *field) {
        F = field;
        t = &(F->t);
    }

    void source::write(const h5cpp::h5group &group) {
        tp->write(group);        
    }
}
