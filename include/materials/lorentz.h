#ifndef GUARD_lorentz_h
#define GUARD_lorentz_h

#include "material.h"
#include "qbox/vec.h"

namespace qbox {

    class lorentz: public material {
    public:
        lorentz(double eps_inf, Array omega_0, Array delta_epsilon, Array gamma);
        lorentz(double eps_inf, double omega_0, double delta_epsilon, double gamma);

        Array alpha(double dt) const;
        Array beta(double dt) const;
        Array delta(double dt) const;

        double Ca_prev(double dt) const;
        double Ca(double dt) const override;
        double Cb(double dt) const override;
        double Da(double dt) const override;
        double Db(double dt) const override;

        void write(const h5cpp::h5group &group) override;

        std::unique_ptr<material> clone() const override;

        int Npoles() const {return omega_0.size();}
        double get_eps_inf() const { return eps_inf; }
        Array get_omega_0() const { return omega_0; }
        Array get_delta_epsilon() const { return delta_epsilon; }
        Array get_gamma() const { return gamma; }

    private:
        double eps_inf;
        Array omega_0;
        Array delta_epsilon;
        Array gamma;
    };

}


#endif
