#include "materials/lorentz.h"

using namespace std;

///Materials

namespace qbox {

    lorentz::lorentz(string name, double eps_inf, Array omega_0, Array delta_epsilon, Array gamma):
        material(name, "lorentz"), eps_inf(eps_inf), omega_0(omega_0), delta_epsilon(delta_epsilon), gamma(gamma) {};

    lorentz::lorentz(double eps_inf, Array omega_0, Array delta_epsilon, Array gamma):
        material("lorentz"), eps_inf(eps_inf), omega_0(omega_0), delta_epsilon(delta_epsilon), gamma(gamma) {};

    lorentz::lorentz(string name, double eps_inf, double omega_0_val, double delta_epsilon_val,
                         double gamma_val):
            lorentz(name, eps_inf, Array::Constant(1,omega_0_val), Array::Constant(1,delta_epsilon_val),
                    Array::Constant(1,gamma_val)) {};

    lorentz::lorentz(double eps_inf, double omega_0_val, double delta_epsilon_val,
                         double gamma_val):
            lorentz(eps_inf, Array::Constant(1,omega_0_val), Array::Constant(1,delta_epsilon_val),
                    Array::Constant(1,gamma_val)) {};

    unique_ptr<material> lorentz::clone() const {
        return unique_ptr<lorentz>(new lorentz(*this));
    }

    void lorentz::write(const h5cpp::h5file &outFile) const {
        auto group = get_group(outFile);

        h5cpp::write_scalar(group_name, group, "material_type");
        h5cpp::write_scalar(eps_inf, group, "eps_inf");
        h5cpp::write_array<double>(omega_0, group, "omega_0");
        h5cpp::write_array<double>(gamma, group, "gamma");
        h5cpp::write_array<double>(delta_epsilon, group, "delta_epsilon");
    }

    Array lorentz::alpha(double dt) const {
        return (2 - omega_0.pow(2)*pow(dt,2))/(1 + gamma*dt);
    }

    Array lorentz::beta(double dt) const {
        return (gamma*dt - 1)/(gamma*dt + 1);
    }

    Array lorentz::delta(double dt) const {
        return (delta_epsilon*omega_0.pow(2)*pow(dt,2))/(1 + gamma*dt);
    }

    double lorentz::Ca_prev(double dt) const {
        double delta_sum = delta(dt).sum();
        return (delta_sum/2)/(2*eps_inf + delta_sum);
    }

    double lorentz::Ca(double dt) const {
        double delta_sum = delta(dt).sum();
        return (2*eps_inf)/(2*eps_inf + delta_sum/2);
    }

    double lorentz::Cb(double dt) const {
        double delta_sum = delta(dt).sum();
        return (2*dt)/(2*eps_inf + delta_sum/2);
    }

    double lorentz::Da(double dt) const {
        return 1;
    }

    double lorentz::Db(double dt) const {
        return dt;
    }

}
