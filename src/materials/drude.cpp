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

        auto dset = group.create_dataset("type", h5cpp::dtype::String, h5cpp::dspace(vector<hsize_t>{1}));
        dset.write(&group_name);
        dset = group.create_dataset("eps_inf", h5cpp::dtype::Double, h5cpp::dspace(vector<hsize_t>{1}));
        dset.write(&eps_inf);
        dset = group.create_dataset("omega_0", h5cpp::dtype::Double, h5cpp::dspace(vector<hsize_t>{Npoles()}));
        dset.write(omega_0.data());
        dset = group.create_dataset("gamma", h5cpp::dtype::Double, h5cpp::dspace(vector<hsize_t>{Npoles()}));
        dset.write(gamma.data());
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
