#ifndef GUARD_lorentz_h
#define GUARD_lorentz_h

#include "material.h"

namespace qbox {

    class lorentz: public material {
    public:
        lorentz(double eps_inf, double omega_0, double delta_epsilon, double gamma);

        double alpha(double dt) const;
        double beta(double dt) const;
        double delta(double dt) const;

        double Ca_prev(double dt) const;
        double Ca(double dt) const override;
        double Cb(double dt) const override;
        double Da(double dt) const override;
        double Db(double dt) const override;

        void write(const h5cpp::h5group &group) override;

        std::unique_ptr<material> clone() const override;
    private:
        double eps_inf;
        double omega_0;
        double delta_epsilon;
        double gamma;
    };

}


#endif
