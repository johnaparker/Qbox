#include "materials/drude.h"

using namespace std;

///Materials

namespace qbox {

    drude::drude(double eps_inf, double omega_0, double gamma): eps_inf(eps_inf), omega_0(omega_0), gamma(gamma) {};

    unique_ptr<material> drude::clone() const {
        return unique_ptr<drude>(new drude(*this));
    }

    void drude::write(const h5cpp::h5group &group) {
    }

    double drude::beta(double dt) const {
        return (pow(omega_0,2)*dt/2)/(1 + gamma*dt/2);
    }

    double drude::kappa(double dt) const {
        return (1 - gamma*dt/2)/(1 + gamma*dt/2);
    }

    double drude::Ca(double dt) const {
        return (2*eps_inf - dt*beta(dt))/(2*eps_inf + dt*beta(dt));
    }

    double drude::Cb(double dt) const {
        return (2*dt)/(2*eps_inf + dt*beta(dt));
    }

    double drude::Da(double dt) const {
        return 1;
    }

    double drude::Db(double dt) const {
        return dt;
    }
}
