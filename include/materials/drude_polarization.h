#ifndef GUARD_drude_polarization_h
#define GUARD_drude_polarization_h

#include "qbox/materials/drude.h"
#include "qbox/materials/polarization.h"

namespace qbox {

    class Field2D;

    class drude_polarization: public polarization {
    public:
        drude_polarization(const grid_properties &grid, const drude &mat);
        void update_J(Field2D &f) override;
        void insert_object(const object &new_object) override;

    private:
        drude mat;
        tensor beta;
    };

}

#endif
