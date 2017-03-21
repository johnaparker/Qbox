#include "geometry/medium.h"

using namespace std;

namespace qbox {

    medium::medium() {};

    bool medium::inside(const vec&) const {
        return true;
    }

    unique_ptr<geometry> medium::clone() const {
        return unique_ptr<medium>(new medium(*this));
    }

}
