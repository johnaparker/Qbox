#include "materials/simple_material.h"

using namespace std;

///Materials

namespace qbox {

    simple_material::simple_material(double eps, double mu, double conduc): eps(eps), mu(mu), conduc(conduc) {};

    unique_ptr<material> simple_material::clone() const {
        return unique_ptr<simple_material>(new simple_material(*this));
    }

}
