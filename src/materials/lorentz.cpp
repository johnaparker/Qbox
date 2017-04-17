#include "materials/lorentz.h"

using namespace std;

///Materials

namespace qbox {

    lorentz::lorentz(double eps_inf, double omega_0, double delta_epsilon, double gamma):
        eps_inf(eps_inf), omega_0(omega_0), delta_epsilon(delta_epsilon), gamma(gamma) {};

    unique_ptr<material> lorentz::clone() const {
        return unique_ptr<lorentz>(new lorentz(*this));
    }

    void lorentz::write(const h5cpp::h5group &group) {
    }

    double lorentz::alpha(double dt) const {
        return (2 - pow(omega_0,2)*pow(dt,2))/(1 + gamma*dt);
    }

    double lorentz::beta(double dt) const {
        return (gamma*dt - 1)/(gamma*dt + 1);
    }

    double lorentz::delta(double dt) const {
        return (delta_epsilon*pow(omega_0,2)*pow(dt,2))/(1 + gamma*dt);
    }

    double lorentz::Ca_prev(double dt) const {
        return (delta(dt)/2)/(2*eps_inf + delta(dt)/2);
    }

    double lorentz::Ca(double dt) const {
        return (2*eps_inf)/(2*eps_inf + delta(dt)/2);
    }

    double lorentz::Cb(double dt) const {
        return (2*dt)/(2*eps_inf + delta(dt)/2);
    }

    double lorentz::Da(double dt) const {
        return 1;
    }

    double lorentz::Db(double dt) const {
        return dt;
    }

}
