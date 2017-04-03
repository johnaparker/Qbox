#ifndef GUARD_simple_material_h
#define GUARD_simple_material_h

#include "materials/material.h"

namespace qbox {

    class simple_material: public material {
    public:
        simple_material(double eps, double mu = 1, double conduc = 0);

        double get_eps() override {return eps;}
        double get_mu() override {return mu;}
        double get_conduc() override {return conduc;}

        void write(const h5cpp::h5group &group) override;

        std::unique_ptr<material> clone() const override;
    private:
        double eps;
        double mu;
        double conduc;
    };

}


#endif
