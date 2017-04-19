#include "materials/drude.h"

using namespace std;

///Materials

namespace qbox {

    drude::drude(double eps_inf, Array omega_0, Array gamma): eps_inf(eps_inf), 
                      omega_0(omega_0), gamma(gamma) {};

    drude::drude(double eps_inf, double omega_0_val, double gamma_val): drude(eps_inf,
                    Array::Constant(1,omega_0_val), Array::Constant(1,gamma_val)) {};

    unique_ptr<material> drude::clone() const {
        return unique_ptr<drude>(new drude(*this));
    }

    void drude::write(const h5cpp::h5group &group) {
    }

    Array drude::beta(double dt) const {
        return (omega_0.pow(2)*dt/2)/(1 + gamma*dt/2);
    }

    Array drude::kappa(double dt) const {
        return (1 - gamma*dt/2)/(1 + gamma*dt/2);
    }

    double drude::Ca(double dt) const {
        double beta_sum = beta(dt).sum();
        return (2*eps_inf - dt*beta_sum)/(2*eps_inf + dt*beta_sum);
    }

    double drude::Cb(double dt) const {
        double beta_sum = beta(dt).sum();
        return (2*dt)/(2*eps_inf + dt*beta_sum);
    }

    double drude::Da(double dt) const {
        return 1;
    }

    double drude::Db(double dt) const {
        return dt;
    }
}
