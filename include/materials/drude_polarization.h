#ifndef GUARD_drude_polarization_h
#define GUARD_drude_polarization_h

#include "qbox/materials/drude.h"
#include "qbox/grid.h"
#include "qbox/object.h"
#include "qbox/vec.h"

namespace qbox {

    class Field2D;

    class drude_polarization {
    public:
        drude_polarization(const grid_properties &grid, const drude &mat);
        void update_J(Field2D &f);
        void insert_object(const object &new_object);

    private:
        grid_properties grid;
        drude mat;

        tensor beta, J;
    };

}

#endif
