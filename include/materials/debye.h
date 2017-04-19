#ifndef GUARD_debye_h
#define GUARD_debye_h

#include "material.h"
#include "qbox/vec.h"

namespace qbox {

    class debye: public material {
    public:
        debye(double eps_inf, Array delta_epsilon, Array tau);
        debye(double eps_inf, double delta_epsilon, double tau);

        Array beta(double dt) const;
        Array kappa(double dt) const;

        double Ca(double dt) const override;
        double Cb(double dt) const override;
        double Da(double dt) const override;
        double Db(double dt) const override;

        void write(const h5cpp::h5group &group) override;

        std::unique_ptr<material> clone() const override;

        int Npoles() const {return delta_epsilon.size();}

    private:
        double eps_inf;
        Array delta_epsilon;
        Array tau;
    };

}


#endif
