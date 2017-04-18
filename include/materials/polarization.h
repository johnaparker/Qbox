#ifndef GUARD_polarization_h
#define GUARD_polarization_h

#include "qbox/grid.h"
#include "qbox/object.h"
#include "qbox/vec.h"

namespace qbox {

    class Field2D;

    class polarization {
    public:
        polarization(const grid_properties &grid): grid(grid) {
            J = tensor(grid.Nx, grid.Ny);
        }
        virtual void update_J(Field2D &f) = 0;
        virtual void insert_object(const object &new_object) = 0;

    protected:
        grid_properties grid;
        tensor J;
    };

}

#endif
