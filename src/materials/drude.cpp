#include "materials/drude.h"

using namespace std;

///Materials

namespace qbox {

    drude::drude(string name, double eps_inf, Array omega_0, Array gamma): material(name, "drude"), eps_inf(eps_inf), 
                      omega_0(omega_0), gamma(gamma) {};

    drude::drude(double eps_inf, Array omega_0, Array gamma): material("drude"), eps_inf(eps_inf), 
                      omega_0(omega_0), gamma(gamma) {};

    drude::drude(string name, double eps_inf, double omega_0_val, double gamma_val): drude(name, eps_inf,
                    Array::Constant(1,omega_0_val), Array::Constant(1,gamma_val)) {};

    drude::drude(double eps_inf, double omega_0_val, double gamma_val): drude(eps_inf,
                    Array::Constant(1,omega_0_val), Array::Constant(1,gamma_val)) {};

    unique_ptr<material> drude::clone() const {
        return unique_ptr<drude>(new drude(*this));
    }

    void drude::write(const h5cpp::h5file &outFile) const {
        auto group = get_group(outFile);

        h5cpp::write_scalar(group_name, group, "material_type");
        h5cpp::write_scalar(eps_inf, group, "eps_inf");
        h5cpp::write_array<double>(omega_0, group, "omega_0");
        h5cpp::write_array<double>(gamma, group, "gamma");
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
