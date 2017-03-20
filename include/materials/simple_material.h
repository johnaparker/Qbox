#ifndef GUARD_simple_material_h
#define GUARD_simple_material_h

#include <memory>
#include "material.h"
#include "vec.h"

namespace qbox {

    class simple_material: public material {
    public:
        simple_material(double eps, double mu);

    private:
        double eps = 1;
        double mu = 1;
    };

}
