#ifndef GUARD_polarization_h
#define GUARD_polarization_h

#include "materials/debye.h"
#include "grid.h"
#include "object.h"
#include "vec.h"

namespace qbox {

    class Field2D;

    class polarization {
    public:
        polarization(const grid_properties &grid, const debye &mat);
        void update_J(Field2D &f);
        void insert_object(const object &new_object);

    private:
        grid_properties grid;
        debye mat;

        tensor beta, prevE, J;
    };

}

#endif
