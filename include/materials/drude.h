#ifndef GUARD_drude_h
#define GUARD_drude_h

#include "material.h"
#include "qbox/vec.h"

namespace qbox {

    class drude: public material {
    public:
        drude(std::string name, double eps_inf, Array omega_0, Array gamma);
        drude(double eps_inf, Array omega_0, Array gamma);

        drude(std::string name, double eps_inf, double omega_0, double gamma);
        drude(double eps_inf, double omega_0, double gamma);

        Array beta(double dt) const;
        Array kappa(double dt) const;

        double Ca(double dt) const override;
        double Cb(double dt) const override;
        double Da(double dt) const override;
        double Db(double dt) const override;

        void write(const h5cpp::h5file &outFile) const override;

        std::unique_ptr<material> clone() const override;

        int Npoles() const {return omega_0.size();}
        double get_eps_inf() const { return eps_inf; }
        Array get_omega_0() const { return omega_0; }
        Array get_gamma() const { return gamma; }

    private:
        double eps_inf;
        Array omega_0;
        Array gamma;
    };

}


#endif
