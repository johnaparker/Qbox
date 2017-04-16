#ifndef GUARD_polarization_h
#define GUARD_polarization_h

#include "materials/debye.h"
#include "grid.h"
#include "field2.h"
#include "vec.h"

namespace qbox {

    class polarization {
    public:
        polarization(grid_properties grid, debye mat);
        void update_E(Field2D &f);
        void store_E(Field2D &f);
        //void insert_object();

    private:
        grid_properties grid;
        debye mat;

        tensor beta, prevE, J;
    };

}

#endif
