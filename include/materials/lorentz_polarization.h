#ifndef GUARD_lorentz_polarization_h
#define GUARD_lorentz_polarization_h

#include "qbox/materials/lorentz.h"
#include "qbox/grid.h"
#include "qbox/object.h"
#include "qbox/vec.h"

namespace qbox {

    class Field2D;

    class lorentz_polarization {
    public:
        lorentz_polarization(const grid_properties &grid, const lorentz &mat);
        void update_J(Field2D &f);
        void insert_object(const object &new_object);

    private:
        grid_properties grid;
        lorentz mat;

        tensor delta, J, prevJ;
    };

}

#endif
