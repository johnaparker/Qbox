#ifndef GUARD_simple_material_h
#define GUARD_simple_material_h

#include "material.h"

namespace qbox {

    class simple_material: public material {
    public:
        simple_material(std::string name, double eps, double mu = 1, double conduc = 0);
        simple_material(double eps, double mu = 1, double conduc = 0);

        double get_eps() const {return eps;}
        double get_mu() const {return mu;}
        double get_conduc() const {return conduc;}

        double Ca(double dt) const override;
        double Cb(double dt) const override;
        double Da(double dt) const override;
        double Db(double dt) const override;

        void write(const h5cpp::h5file &outFile) const override;

        std::unique_ptr<material> clone() const override;
    private:
        double eps;
        double mu;
        double conduc;
    };

}


#endif
