#ifndef GUARD_debye_h
#define GUARD_debye_h

#include "material.h"

namespace qbox {

    class debye: public material {
    public:
        debye(double eps_inf, double delta_epsilon, double tau);

        double beta(double dt) const;
        double kappa(double dt) const;

        double Ca(double dt) const override;
        double Cb(double dt) const override;
        double Da(double dt) const override;
        double Db(double dt) const override;

        void write(const h5cpp::h5group &group) override;

        std::unique_ptr<material> clone() const override;
    private:
        double eps_inf;
        double delta_epsilon;
        double tau;
    };

}


#endif
