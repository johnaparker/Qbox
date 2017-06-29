#ifndef GUARD_pml_h
#define GUARD_pml_h

#include <memory>
#include "vec.h"
#include "grid.h"
#include "field2.h"

namespace qbox {

    //PML boundary object
    class pml {
    public:
        pml() = default;
        pml(grid_properties grid, int m = 3, int ma = 1, double k_max = 1, double a_max = 0.2, double sig_scale = 1);

        double b_func(double t);
        double c_func(double t);

        double a_func(double t);
        double k_func(double t);
        double sig_func(double t);

        void set_scaling_factors(Array &kedx, Array &khdx, Array &kedy, Array &khdy);
        void update_E(Field2D &f);
        void update_H(Field2D &f);

    private:
        grid_properties grid;
        int thickness;   //# of cells for thickness

        int m, ma;
        double k_max, sig_max, a_max;

        Array be_x, be_y, ce_x, ce_y;
        Array bh_x, bh_y, ch_x, ch_y;

        tensor psi_Ezx1, psi_Ezx2;
        tensor psi_Ezy1, psi_Ezy2;
        tensor psi_Hxy1, psi_Hxy2;
        tensor psi_Hyx1, psi_Hyx2;
    };
}

#endif
