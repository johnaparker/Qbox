#ifndef GUARD_debye_polarization_h
#define GUARD_debye_polarization_h

#include "qbox/materials/debye.h"
#include "qbox/materials/polarization.h"

namespace qbox {

    class Field2D;

    class debye_polarization: public polarization {
    public:
        debye_polarization(const grid_properties &grid, const debye &mat);
        void update_J(Field2D &f) override;
        void insert_object(const object &new_object) override;
        void reset() override;

    private:
        debye mat;
        tensor3 beta;
    };

}

#endif
