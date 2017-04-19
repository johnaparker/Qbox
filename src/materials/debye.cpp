#include "materials/debye.h"

using namespace std;

///Materials

namespace qbox {

    debye::debye(double eps_inf, Array delta_epsilon, Array tau): eps_inf(eps_inf),
                    delta_epsilon(delta_epsilon), tau(tau) {};

    debye::debye(double eps_inf, double delta_epsilon_val, double tau_val): debye(eps_inf,
            Array::Constant(1,delta_epsilon_val), Array::Constant(1,tau_val)) {};

    unique_ptr<material> debye::clone() const {
        return unique_ptr<debye>(new debye(*this));
    }

    void debye::write(const h5cpp::h5group &group) {
    }

    Array debye::beta(double dt) const {
        return (delta_epsilon*dt/tau)/(1 + dt/(2*tau));
    }

    Array debye::kappa(double dt) const {
        return (1 - dt/(2*tau))/(1 + dt/(2*tau));
    }

    double debye::Ca(double dt) const {
        return 1;
    }

    double debye::Cb(double dt) const {
        double beta_sum = beta(dt).sum();
        return (2*dt)/(2*eps_inf + beta_sum);
    }

    double debye::Da(double dt) const {
        return 1;
    }

    double debye::Db(double dt) const {
        return dt;
    }
}
