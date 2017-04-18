#ifndef GUARD_lorentz_polarization_h
#define GUARD_lorentz_polarization_h

#include "qbox/materials/lorentz.h"
#include "qbox/materials/polarization.h"

namespace qbox {

    class Field2D;

    class lorentz_polarization: public polarization {
    public:
        lorentz_polarization(const grid_properties &grid, const lorentz &mat);
        void update_J(Field2D &f) override;
        void insert_object(const object &new_object) override;

    private:
        lorentz mat;
        tensor delta, prevJ;
    };

}

#endif
